# Yutovo Project Agent Notes

## Project Structure
- `yutovo-calculator/` — Core math engine (Real, Complex, Integer, Rational, Symbolic)
- `yutovo-solver/` — Solver service (WebSocket-based calculation backend)
- `yutovo-editor/` — Document editor with MathML rendering and solver integration
- `yutovo-desktop/` — Desktop GUI application using `yutovo-editor` and `yutovo-solver`

## External Dependencies
- `../third-party/giac-2.0.0/` — Giac source code (used for symbolic integration and CAS operations)

## Agent Rules

### Mandatory tooling
- **Always use repowise — this rule is mandatory.** For every codebase question, exploration, or before modifying files, you must use repowise tools (`get_answer`, `get_context`, `get_risk`, `search_codebase`, etc.) first. Do not use raw `grep`, `Read`, or `Glob` as the primary way to understand code, find symbols, or assess risk. It is acceptable to use raw tools only to read a file path or line range that repowise has already identified.
- **Never delete files without explicit user permission.** Do not remove source files, test files, core dumps, logs, build artifacts, or any other files unless the user explicitly asks for it. When in doubt, leave the file in place and ask.
- **Never create separate namespaces (such as `namespace detail` or anonymous namespaces) without explicit user permission.** Helper functions should be placed in the common `yutovo_calculator` namespace, for example in `utils.h/utils.cpp` or `giac_utils.h/giac_utils.cpp`, or as `static` methods of the appropriate class.
- **Never commit without explicit user permission.** Do not run `git commit`, `git push`, `git reset`, `git rebase`, or any other git mutations unless explicitly asked to do so. Ask for confirmation each time when git mutations are needed.
- **Never create branches or delete stashes without explicit user permission.** Do not run `git branch`, `git checkout -b`, `git stash branch`, `git stash drop`, `git stash pop`, `git stash clear`, or similar commands that create branches or remove stashes unless the user explicitly asked for it.
- **Never delete existing tests.** When fixing regressions or refactoring, update test expectations to match the new correct behavior, but do not remove tests. If `git checkout` or similar commands are used to revert a file, verify that no user-added tests were lost.

### Code Style
Always place braces on their own line for control structures.

Never leave two blank lines in a row — use at most one blank line between definitions or statements.

For constructors, place the colon of the initializer list on the same line as the constructor signature. Put each initializer/base on its own line with a normal 4-space indent, and place the opening brace on its own line:
```cpp
// CORRECT
ExpressionNode(const DefiniteIntegralNode<Number>& node)
    : first(node)
{
}

// WRONG
ExpressionNode(const DefiniteIntegralNode<Number>& node) : first(node) {}
```

If a function has a definition, separate it from surrounding code with blank lines before and after:
```cpp
// CORRECT
ExpressionNode(const DefiniteIntegralNode<Number>& node)
    : first(node)
{
}

ExpressionNode(const DerivativeAtPointNode<Number>& node)
    : first(node)
{
}

// WRONG
ExpressionNode(const DefiniteIntegralNode<Number>& node)
    : first(node)
{
}
ExpressionNode(const DerivativeAtPointNode<Number>& node)
    : first(node)
{
}
```

### Naming
Avoid abbreviations in identifiers; use full words. For example, prefer `Context` over `Ctx`, `Symbols` over `Syms`, etc.

### Number-type classes
All functions belonging to a number type (`Real`, `Complex`, `Array`, `Matrix`, ...) must be defined **inside the class body**, never as free functions after it — follow the `array.h` pattern:
- Functions taking the type as an argument (math builtins like `sin`/`det`/`size`, operators, internal helpers like `ForEach`) are **friend functions defined inline in the class** (hidden friends).
- Helpers operating on the element type (e.g. `MatrixElementToInt`) are **static member functions** — hidden friends with element-type arguments are not findable by name lookup, because ADL does not associate the element type with the class.
- The only namespace-scope declarations allowed are **non-defining forward declarations in `parser.cpp`** for the functions registered via `&func` pointers as builtins (hidden friends are otherwise invisible to ordinary lookup).

### Parenthesized expressions
Keep the contents of parentheses (function argument lists, conditions, initializers, etc.) on a single line when it fits. Only wrap to a new line if the expression would exceed **140 columns**.
When a parenthesized expression is wrapped, each continuation line uses the normal **4-space indent**; do not align arguments with the opening parenthesis.
When a long expression wraps, break the line **after an operator** (`|`, `>>`, `,`, `&&`, etc.) so the operator stays at the end of the continued line; never put a line break between an operator and its operand — do not write `x = guard >>` on one line with the parenthesized operand starting the next line.
When editing existing code, do not reformat or rewrap surrounding lines to match a new style — apply the edit in place, keeping the existing line layout.
```cpp
// CORRECT
void ShortFunction(int a, int b, int c);

void LongFunctionName(const std::u32string& first_argument, const std::u32string& second_argument,
    int third_argument);

auto result = SomeFunction(first_argument, second_argument,
    third_argument, fourth_argument);

if (condition_a && condition_b)
{
    // ...
}

unary = recursion_guard >> (hold[derivative_at_point] | hold[evaluate_at_point] |
    number | function_call | identifier | '(' > expression > ')');

// WRONG
void LongFunctionName(
    const std::u32string& first_argument,
    const std::u32string& second_argument,
    int third_argument);

void LongFunctionName(const std::u32string& first_argument,
                      const std::u32string& second_argument,
                      int third_argument);

unary = recursion_guard >>
    (hold[derivative_at_point] | hold[evaluate_at_point] |
    number | function_call | identifier | '(' > expression > ')');

try {
    // ...
} catch (...) {
    // ...
}
```

### Lambdas
Place the capture clause on a new line, indented by 4 spaces. Parameters, the `->` return type, and the opening brace follow the normal rules: parameters and return type stay on the same line as the capture clause, and the opening brace goes on its own line.
```cpp
// CORRECT
auto callback =
    [](int value) -> bool
    {
        return value > 0;
    };

auto reference =
    [&]() -> void
    {
        DoWork();
    };

// WRONG
auto callback = [](int value) -> bool {
    return value > 0;
};

auto callback =
    [](int value) -> bool {
    return value > 0;
};
```

### Current Work Status
The project work journal (feature notes, critical files changed, editor test patterns, blockers) lives in the `yutovo-current-work` skill (`.zcode/skills/yutovo-current-work/SKILL.md`). Invoke it before modifying yutovo-calculator / yutovo-solver / yutovo-editor / yutovo-desktop sources, and update it whenever a change alters behavior, files, tests, or blockers — do not grow AGENTS.md with per-feature changelogs.

## Build
Each component is built and tested from its own `build/debug` subdirectory (in-tree builds are not used):
```bash
cd yutovo-calculator/build/debug && make -j$(lscpu -p=Core,Socket | grep -v '^#' | sort -u | wc -l) yutovo-calculator_tests
./test/yutovo-calculator_tests

cd yutovo-solver/build/debug && make -j$(lscpu -p=Core,Socket | grep -v '^#' | sort -u | wc -l)

cd yutovo-editor/build/debug && make -j$(lscpu -p=Core,Socket | grep -v '^#' | sort -u | wc -l) yutovo-editor_tests
./test/yutovo-editor_tests

cd yutovo-desktop/build/debug && make -j$(lscpu -p=Core,Socket | grep -v '^#' | sort -u | wc -l) yutovo-desktop

# Emscripten/wasm build (from yutovo-calculator)
cd yutovo-calculator/build_web/debug && make -j$(lscpu -p=Core,Socket | grep -v '^#' | sort -u | wc -l) yutovo-calculator
```
Use the number of **physical** cores for `-j` (`lscpu -p=Core,Socket | grep -v '^#' | sort -u | wc -l`), not `nproc` — `nproc` counts hyperthreads too and the build is slower with them.

### Test runtime
Running the full `yutovo-editor_tests` suite takes approximately **25 minutes** (symbolic tests are particularly slow due to WebSocket solver round-trips).
