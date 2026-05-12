# Yutovo Project Agent Notes

## Project Structure
- `yutovo-calculator/` — Core math engine (Real, Complex, Integer, Rational, Symbolic)
- `yutovo-solver/` — Solver service (WebSocket-based calculation backend)
- `yutovo-editor/` — Document editor with MathML rendering and solver integration
- `yutovo-desktop/` — Desktop GUI application using `yutovo-editor` and `yutovo-solver`

## Current Work: Symbolic Integration

### yutovo-calculator
- **Class `Symbolic`** (`src/symbolic.h/cpp`) wraps `SymEngine::Expression` via `std::unique_ptr`.
- `Parser<Symbolic>`, `Solver<Symbolic>`, `Expression<Symbolic>` are specialized.
- `^` is parsed in the generic grammar; power output uses `pow(base,exp)` via `ReplacePowerOperator`.
- `evalf` returns `Symbolic`; explicit `to_real()` / `to_complex()` perform casting.
- `factor()` and `integrate()` throw `NotImplemented = 302` (not available in SymEngine 0.14.0).
- `sqrt(x)` is implemented for all symbolic types via `root(x, 2)` (i.e. `pow(x, 1/2)`). Friend function added in `symbolic.h`; registered in `Parser<Symbolic<...>>` constructors in `parser.cpp`.
- `HAVE_SYMENGINE_MPFR` is commented out in `symengine_config.h`; `evalf` falls back to 53-bit double.
- `Symbolic<Number>::ToJson()` recursively traverses the SymEngine AST and emits JSON using yutovo-editor element type codes (7=CODE_ROW, 8=CODE_STRING, 10=SHAPE, 11=PLUS, 12=MINUS, 13=MULTIPLY, 14=DIVISION, 15=POWER, 45-47=SYMBOLIC_*_RESULT). This replaces the fragile string-parsing approach in `yutovo-editor::AddSymbolicElements`.

### yutovo-solver
- `ResultType::SYMBOLIC` added in `types.h`.
- `CalculatorSolver` has `symbolic_parser` and `SolveSymbolic()`.
- `AUTO` mode tries `SYMBOLIC` last in `results_order[6]`.
- `CMakeLists.txt` links `libmpfr.a`, `libgmp.a`, `${SYMENGINE_LIBRARIES}`.

### yutovo-editor
- `ElementType::SYMBOLIC_RESULT` added.
- `Config::SymbolicResultConfig` (default `precision=10`) in `config.h/cpp`.
- `SymbolicResult` (`formulas/result.h/cpp`) parses the solver result string and builds formula elements (`CodeString`, `Plus`, `Minus`, `Multiply`, `OpenBracket`, `CloseBracket`, `Comma`) inside `ResultRow`.
- `SymbolicSolverTask` sends JSON with `result_type=7` (`SYMBOLIC`) and `symbolic_precision`.
- `Equation::UpdateResult` **must** handle `ResultType::SYMBOLIC` by creating `SymbolicResult`; missing this causes tests to hang because no solver task is ever dispatched.
- `ResultTask::Execute` **must** handle `ElementType::SYMBOLIC_RESULT` to deliver the solver response to `SymbolicResult::PutResult`; missing this leaves the waiting symbol (`~`) forever.

### yutovo-desktop
- `CMakeLists.txt` needs `find_package(SymEngine REQUIRED)` and `${SYMENGINE_LIBRARIES}` in `target_link_libraries` for Linux, because static libraries (`yutovo-calculator`, `yutovo-solver`) now depend on SymEngine.
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
Use `-j16` maximum for building to avoid OOM kills. Build from the `build/debug` subdirectory:
```bash
cd yutovo-calculator/build/debug && make -j16 yutovo-calculator_tests
cd yutovo-solver/build/debug && make -j16

cd yutovo-editor/build/debug && make -j16 yutovo-editor_tests
cd yutovo-desktop/build/debug && make -j16 yutovo-desktop
```

### Test runtime
Running the full `yutovo-editor_tests` suite takes approximately **25 minutes** (symbolic tests are particularly slow due to WebSocket solver round-trips).

## Critical Files Changed
- `yutovo-editor/src/formulas/equation.cpp` — added `ResultType::SYMBOLIC` case in `UpdateResult`
- `yutovo-editor/src/task.cpp` — added `ElementType::SYMBOLIC_RESULT` case in `ResultTask::Execute`
- `yutovo-editor/src/formulas/result.cpp` — `SymbolicResult::AddSymbolicElements` parses result string into formula elements
- `yutovo-editor/test/solver_symbolic.cpp` — rewritten to use proper element insertion and exact HTML checks, including undo assertions; added `solver6` (fraction result), `solver7` (complex result), `solver8` (power result `1/x`), `solver9` (power result `x^-1`)
- `yutovo-editor/test/solver_auto.cpp` — added 5 AUTO-mode symbolic fallback tests (`symbolic1..symbolic5`) using `ToText()` checks; added `symbolic6` (fraction) and `symbolic7` (complex)
- `yutovo-desktop/src/CMakeLists.txt` — added `find_package(SymEngine)` and `${SYMENGINE_LIBRARIES}` in `target_link_libraries`
- `yutovo-desktop/test/CMakeLists.txt` — added `find_package(SymEngine)` and `${SYMENGINE_LIBRARIES}` in `target_link_libraries`
- `yutovo-desktop/src/result_settings_form.ui` / `.cpp` — added "Symbolic" tab with precision
- `yutovo-desktop/src/document_window.cpp` / `.h` — `Present as → Symbolic` is now a submenu containing checkable items `Real`, `Rational`, `Complex`
- `yutovo-desktop/src/command_map.cpp` — added `\eq_sym` command
- `yutovo-desktop/src/mainwindow.cpp` — added symbolic result in settings load/save
- `yutovo-desktop/src/translations/*.ts` — added `Symbolic` translations

- `yutovo-calculator/test/symbolic.cpp` — added `variables_rational`, `variables_complex`, `user_functions_rational`, `user_functions_complex` tests
- `yutovo-calculator/src/symbolic.cpp` — fixed `ReplacePowerOperator` to correctly handle parenthesized exponents (e.g. `x**(-1)` → `pow(x,-1)`)
- `yutovo-editor/src/formulas/result.cpp` — `SymbolicResult::AddSymbolicElements` now parses `pow(base,exp)` into `Power` formula elements
- `yutovo-editor/src/formulas/power.h` — added `GetBaseRow()` and `GetExponentRow()` public helpers

## Next Steps / Blockers
- All new tests pass. Calculator: 498 tests, Editor: 16 symbolic tests (9 direct + 7 auto fallback).
