# Yutovo Project Agent Notes

## Project Structure
- `yutovo-calculator/` — Core math engine (Real, Complex, Integer, Rational, Symbolic)
- `yutovo-solver/` — Solver service (WebSocket-based calculation backend)
- `yutovo-editor/` — Document editor with MathML rendering and solver integration
- `yutovo-desktop/` — Desktop GUI application using `yutovo-editor` and `yutovo-solver`

## Agent Rules

- **Never delete files without explicit user permission.** Do not remove source files, test files, core dumps, logs, build artifacts, or any other files unless the user explicitly asks for it. When in doubt, leave the file in place and ask.
- **Never create separate namespaces (such as `namespace detail` or anonymous namespaces) without explicit user permission.** Helper functions should be placed in the common `yutovo_calculator` namespace, for example in `utils.h/utils.cpp` or `giac_utils.h/giac_utils.cpp`, or as `static` methods of the appropriate class.
- **Never commit without explicit user permission.** Do not run `git commit`, `git push`, `git reset`, `git rebase`, or any other git mutations unless explicitly asked to do so. Ask for confirmation each time when git mutations are needed.
- **Never delete existing tests.** When fixing regressions or refactoring, update test expectations to match the new correct behavior, but do not remove tests. If `git checkout` or similar commands are used to revert a file, verify that no user-added tests were lost.

### Code Style
Always place braces on their own line for control structures:

### Parenthesized expressions
Keep the contents of parentheses (function argument lists, conditions, initializers, etc.) on a single line. Only wrap to a new line if the expression would exceed **150 columns**.
```cpp
// CORRECT
try
{
    // ...
}
catch (...)
{
    // ...
}

// WRONG
try {
    // ...
} catch (...) {
    // ...
}
```

## Current Work: Symbolic Integration

### yutovo-calculator
- **Class `Symbolic`** (`src/symbolic.h/cpp`) wraps `giac::gen` via `std::unique_ptr`.
- `Parser<Symbolic>`, `Solver<Symbolic>`, `Expression<Symbolic>` are specialized for `Symbolic<Real>`, `Symbolic<Rational>`, and `Symbolic<Complex>`.
- `^` is parsed left-associatively in the generic grammar. Giac flattens `(x^y)^z` to `x^(y*z)`; the formatter reconstructs `pow(pow(x,y),z)` for left-associative chains and prints explicit right-associative chains like `x^(y^z)` as `pow(x,y**z)`.
- `evalf` returns `Symbolic`; explicit `to_real()` / `to_complex()` perform casting.
- `factor()` and `integrate()` throw `NotImplemented = 302`.
- `sqrt(x)` is implemented for all symbolic types via `root(x, 2)` (i.e. `pow(x, 1/2)`). `sqrt(-∞)` returns `nan` for Real/Complex and throws for Rational.
- `cot(x)`, `sec(x)`, `csc(x)`, `coth(x)`, `sech(x)`, `csch(x)` are rendered as inert functions (`yut_cot`, etc.) so that poles map to `∞` instead of being simplified away.
- Inverse hyperbolic functions (`asinh`, `acosh`, `atanh`, `acoth`, `asech`, `acsch`) and their `arc...`/`ars...` synonyms are registered for all symbolic parsers. Inert wrappers preserve names; singularities such as `acoth(1)` and `acsch(0)` evaluate to `∞` for Real/Complex and remain symbolic for Rational.
- `fact(x)` (postfix `!`) is implemented for all symbolic types as a product `1*2*...*n` for non-negative integer arguments; returns `factorial(x)` for symbolic/non-integer arguments.
- `Symbolic<Number>::ToJson()` builds an AST from the giac-printed string and emits JSON using yutovo-editor element type codes (7=CODE_ROW, 8=CODE_STRING, 10=SHAPE, 11=PLUS, 12=MINUS, 13=MULTIPLY, 14=DIVISION, 15=POWER, 16=SQUARE_ROOT, 17=NTH_ROOT, 45-47=SYMBOLIC_*_RESULT). Division operands are wrapped in a single `CODE_ROW`; scientific numbers are emitted as flat elements so they do not add extra nesting inside sums or products.
- `subs` uses `giac::limit` to detect essential singularities of `exp`, `sin`, `cos`, `sinh`, `cosh` and returns `nan` for Real/Complex (throws for Rational).
- All giac-specific helpers live in `src/giac_utils.h` / `src/giac_utils.cpp` in the common `yutovo_calculator` namespace; `src/utils.h` / `src/utils.cpp` contain only general-purpose utilities.
- `WrongArgumentsCount` exceptions report a `size` that covers the full function call up to and including the closing parenthesis. `ExpressionPosition::size` is populated by `Annotation` for function-call nodes, and `Solver::CallSize()` returns it (falling back to the identifier length when unset).
- `RealNumberStr` evaluates rational strings such as `1333/1000` with MPFR so that `CombineLikeTerms` does not turn decimal reciprocals into bogus scientific notation (e.g., `1.333E+3`). `AddCoeffs` / `MultiplyCoeffs` fall back to `giac::gen` for decimal or scientific-notation coefficients.
- On Linux `yutovo-calculator` links the **system** MPFR/GMP libraries and a static **giac** library found in `${INSTALL_PATH}/lib`. Debug builds must link `${INSTALL_PATH}/lib/libgiacd.a` and release builds `${INSTALL_PATH}/lib/libgiac.a`; mixing configurations causes an ABI mismatch and memory corruption inside giac (e.g., crashes in `giac::expand`). The Emscripten/wasm build links `${INSTALL_PATH}/wasm/libgiac.a`, `${INSTALL_PATH}/wasm/libmpfr.a`, `${INSTALL_PATH}/wasm/libgmp.a`, and `${INSTALL_PATH}/wasm/libgmpxx.a`, and compiles `src/symbolic.cpp` against the wasm build of giac; the former header-only stub has been removed.
- Linux consumers of the installed `yutovo-calculator` target must also call `pkg_check_modules(mpfr REQUIRED IMPORTED_TARGET mpfr)` and `pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)` because they are recorded in the imported target's `INTERFACE_LINK_LIBRARIES`.

### yutovo-solver
- `ResultType::SYMBOLIC` added in `types.h`.
- `CalculatorSolver` has `symbolic_parser` and `SolveSymbolic()`.
- `AUTO` mode tries `SYMBOLIC` last in `results_order[6]`.
- On Linux `yutovo-solver` does **not** link GMP/MPFR directly; it inherits them transitively from `yutovo-calculator` / giac.

### yutovo-editor
- `ElementType::SYMBOLIC_RESULT` added.
- `Config::SymbolicResultConfig` (default `precision=10`) in `config.h/cpp`.
- `SymbolicResult` (`formulas/result.h/cpp`) parses the solver result string and builds formula elements (`CodeString`, `Plus`, `Minus`, `Multiply`, `OpenBracket`, `CloseBracket`, `Comma`) inside `ResultRow`.
- `SymbolicSolverTask` sends JSON with `result_type=7` (`SYMBOLIC`) and `symbolic_precision`.
- `Equation::UpdateResult` **must** handle `ResultType::SYMBOLIC` by creating `SymbolicResult`; missing this causes tests to hang because no solver task is ever dispatched.
- `ResultTask::Execute` **must** handle `ElementType::SYMBOLIC_RESULT` to deliver the solver response to `SymbolicResult::PutResult`; missing this leaves the waiting symbol (`~`) forever.

### yutovo-desktop
- `src/CMakeLists.txt` locates the static **giac** library with `find_library(GIAC_LIBRARY ...)` and creates an imported `giac_imported` target for Linux, because static libraries (`yutovo-calculator`, `yutovo-solver`) now depend on giac.
- Same for `test/CMakeLists.txt`.
- Added "Symbolic" tab in `ResultSettingsForm` (precision setting).
- Added `Present as → Symbolic` in context menu (`DocumentWindow`).
- Added `\eq_sym` command in `CommandMap`.
- Added `Symbolic` translations in `.ts` files (ru: Символьные, es: Simbólicos, pt_BR: Simbólicos).

## Editor Test Patterns

### Entering expressions in code blocks
Do **not** put operators inside `InsertString`:
```cpp
// WRONG
 document.InsertString("x+1", true);

// CORRECT
 document.InsertString("x", true);
 document.InsertPlus(true);
 document.InsertString("1", true);
```

Use dedicated insert methods for operators:
- `InsertPlus(true)` / `InsertMinus(true)`
- `InsertMultiply(true)` / `InsertDivision(true)`
- `InsertPower(true)` — wraps the preceding string element into the base automatically; caret moves to exponent.
- `InsertOpenRoundBracket(true)` / `InsertCloseRoundBracket(true)`
- `InsertComma(true)`

Functions without dedicated inserts (e.g. `sin`, `diff`, `expand`, `subs`) are entered as strings followed by brackets.

### Power behavior
`InsertPower(true)` after a string moves that string into the base and places the caret in the exponent.
Example:
```cpp
document.InsertString("x", true);
document.InsertPower(true);
document.InsertString("2", true);
// Parser text: pow(x,2)
```

### Checking results
Check full `ToHtml()` exactly like `SolverSymbolicTest::solver1`:
```cpp
ASSERT_TRUE(document.ToHtml() ==
    "<body>"
        "<p>"
            "<math xmlns='http://www.w3.org/1998/Math/MathML'>"
                "<mrow>"
                    "<mrow>"
                        "<mi>x</mi>"
                        "<mo>+</mo>"
                        "<mi>1</mi>"
                    "</mrow>"
                    "<mo>=</mo>"
                    "<mrow>"
                        "<mrow>"
                            "<mi>1</mi>"
                            "<mo>+</mo>"
                            "<mi>x</mi>"
                        "</mrow>"
                    "</mrow>"
                "</mrow>"
            "</math>"
        "</p>"
    "</body>") << document.ToHtml();
```

HTML structure:
- `CodeParagraph` → `<math xmlns='http://www.w3.org/1998/Math/MathML'>`
- `CodeRow` → `<mrow>`
- `CodeString` → `<mi>text</mi>`
- `Plus` / `Minus` → `<mo>+</mo>` / `<mo>-</mo>`
- `Multiply` → `<mo>×</mo>` (not `<mo>*</mo>`)
- `Power` → `<msup><mrow>base</mrow><mrow>exp</mrow></msup>`
- `SymbolicResult` (inside last `CodeRow`) produces `<mrow><mrow>elements...</mrow></mrow>` because `ResultRow` (`CodeColumn`) wraps its `CodeRow` content.

### Undo checks
If the test calls `Undo()`, always assert the document state after undo:
```cpp
document.Undo();
document.WaitUndo();
std::this_thread::sleep_for(200ms);
ASSERT_TRUE(document.ToHtml() == "<body>...</body>") << document.ToHtml();
```

### Build
Each component is built and tested from its own `build/debug` subdirectory (in-tree builds are not used):
```bash
cd yutovo-calculator/build/debug && make -j4 yutovo-calculator_tests
./test/yutovo-calculator_tests

cd yutovo-solver/build/debug && make -j4

cd yutovo-editor/build/debug && make -j4 yutovo-editor_tests
./test/yutovo-editor_tests

cd yutovo-desktop/build/debug && make -j4 yutovo-desktop

# Emscripten/wasm build (from yutovo-calculator)
cd yutovo-calculator/build_web/debug && make -j4 yutovo-calculator
```
Use `-j4` maximum for building on any platform.

### Test runtime
Running the full `yutovo-editor_tests` suite takes approximately **25 minutes** (symbolic tests are particularly slow due to WebSocket solver round-trips).

## Critical Files Changed
- `yutovo-editor/src/formulas/equation.cpp` — added `ResultType::SYMBOLIC` case in `UpdateResult`
- `yutovo-editor/src/task.cpp` — added `ElementType::SYMBOLIC_RESULT` case in `ResultTask::Execute`
- `yutovo-editor/src/formulas/result.cpp` — `SymbolicResult::AddSymbolicElements` parses result string into formula elements
- `yutovo-editor/test/solver_symbolic.cpp` — rewritten to use proper element insertion and exact HTML checks, including undo assertions; added `solver6` (fraction result), `solver7` (complex result), `solver8` (power result `1/x`), `solver9` (power result `x^-1`)
- `yutovo-editor/test/solver_auto.cpp` — added 5 AUTO-mode symbolic fallback tests (`symbolic1..symbolic5`) using `ToText()` checks; added `symbolic6` (fraction) and `symbolic7` (complex)
- `yutovo-desktop/src/CMakeLists.txt` — added `find_library(GIAC_LIBRARY ...)` and imported `giac_imported` target
- `yutovo-desktop/test/CMakeLists.txt` — added `find_library(GIAC_LIBRARY ...)` and imported `giac_imported` target
- `yutovo-desktop/src/result_settings_form.ui` / `.cpp` — added "Symbolic" tab with precision
- `yutovo-desktop/src/document_window.cpp` / `.h` — `Present as → Symbolic` is now a submenu containing checkable items `Real`, `Rational`, `Complex`
- `yutovo-desktop/src/command_map.cpp` — added `\eq_sym` command
- `yutovo-desktop/src/mainwindow.cpp` — added symbolic result in settings load/save
- `yutovo-desktop/src/translations/*.ts` — added `Symbolic` translations

- `yutovo-calculator/src/giac_utils.h` / `giac_utils.cpp` — new files holding giac-specific helpers (moved from `symbolic.h`/`symbolic.cpp`)
- `yutovo-calculator/src/utils.h` / `utils.cpp` — restored to general-purpose utilities
- `yutovo-calculator/src/ast.h` — added `ExpressionPosition::size`
- `yutovo-calculator/src/annotation.h` — `Annotation` now computes `size` for function-call nodes by scanning from the identifier to the matching closing fence
- `yutovo-calculator/src/solver.h` / `solver.cpp` — added `Solver::CallSize()`; all `WrongArgumentsCount` throws now use `CallSize(op)`
- `yutovo-calculator/src/CMakeLists.txt` — builds and installs `giac_utils.h` / `giac_utils.cpp`
- `yutovo-calculator/test/real.cpp` — updated `CalcTestReal.errors5` to expect `size == 6` for `sqrt();`
- `yutovo-calculator/test/symbolic_real.cpp` — added `CalcTestSymbolicReal.simplify_wrong_args` regression test
- `yutovo-calculator/test/symbolic.cpp` — added `variables_rational`, `variables_complex`, `user_functions_rational`, `user_functions_complex` tests
- `yutovo-calculator/src/symbolic.cpp` — fixed `ReplacePowerOperator` to correctly handle parenthesized exponents (e.g. `x**(-1)` → `pow(x,-1)`)
- `yutovo-calculator/src/parser.cpp` — registered `asinh`/`acosh`/`atanh`/`acoth`/`asech`/`acsch` and `arcsinh`/`arccosh`/`arctanh`/`arccoth`/`arcsech`/`arccsch`/`arccosech` synonyms for symbolic parsers
- `yutovo-calculator/test/symbolic_real.cpp`, `symbolic_rational.cpp`, `symbolic_complex.cpp` — added `hyperbolic_inverse_synonyms` tests
- `yutovo-editor/src/formulas/result.cpp` — `SymbolicResult::AddSymbolicElements` now parses `pow(base,exp)` into `Power` formula elements
- `yutovo-editor/src/formulas/power.h` — added `GetBaseRow()` and `GetExponentRow()` public helpers

## Next Steps / Blockers
- All Linux calculator tests pass (1116 tests across Real, Complex, Integer, Rational, Array, Symbolic, etc.).
- `yutovo-solver` tests pass (40 tests).
- The three failing `SolverSymbolicTest` editor tests (`solver17`, `solver18`, `solver22`) have been fixed by adjusting calculator JSON output; no editor tests were modified.
- The Emscripten/wasm symbolic stub is implemented; native behavior is unchanged.
