# Compiler-Based Code Complexity Analyzer

## Project Overview

A Compiler Construction term project that applies the front-end phases of a
compiler — lexical analysis, syntax analysis (with an actual parse tree),
and structural analysis — to a simplified C-like language, in order to
measure and report a program's structural complexity.

Unlike a traditional compiler, this system does not generate machine code.
It stops after building the parse tree and analyzing it, producing a
complexity report instead.

**Supported language subset:** a single parameterless function definition
containing variable declarations, arithmetic/logical expressions,
assignment (including `+=`, `-=`, `*=`, `/=`), increment/decrement,
`if` / `else if` / `else`, `while`, `for`, and `return`.

---

## Project Objectives

- Apply compiler construction concepts practically.
- Implement lexical analysis that produces a correct, complete token stream.
- Implement a recursive-descent parser that **builds an actual parse tree**
  (not just validates tokens) and reports syntax errors with line numbers.
- Analyze the parse tree to extract control-flow structure.
- Calculate a complexity score from that structure.
- Generate a readable complexity report.

---

## Architecture

```
   Source Code
        |
        v
  Lexical Analyzer  -->  Token Stream (with line numbers)
        |
        v
      Parser         -->  Parse Tree (AST)
        |
        v
 Structural Analyzer -->  Metrics (decision points, nesting depth...)
        |
        v
  Report Generator   -->  Complexity Report
```

Each phase is a separate translation unit:

| File            | Responsibility                                             |
| ---------------- | ----------------------------------------------------------- |
| `lexer.h/.cpp`   | Source text -> token stream. Tracks line numbers.            |
| `ast.h/.cpp`     | Parse tree node definition + pretty-printer.                |
| `parser.h/.cpp`  | Recursive-descent parser with operator-precedence expression parsing. Builds the AST, throws `ParseError` on invalid syntax. |
| `analyzer.h/.cpp`| Walks the AST to compute structural metrics and the complexity score. |
| `report.h/.cpp`  | Formats and prints the final report.                        |
| `main.cpp`       | Wires the phases together; catches `LexError`/`ParseError`. |

---

## Lexical Analysis

Token types: `KEYWORD`, `IDENTIFIER`, `NUMBER`, `OPERATOR`, `DELIMITER`,
`STRING`, `EOF_TOKEN`. Each token also carries the source line it came from.

Handles, correctly (verified — see Testing section):
- Identifiers with underscores (`my_count`, not just `mycount`)
- Multi-character operators: `== != <= >= && || ++ -- += -= *= /=`
- Integer and float literals (`10`, `3.14`)
- Line comments `//` and block comments `/* ... */`
- Unrecognized characters raise a `LexError` with the line number instead
  of being silently dropped.

## Syntax Analysis

A genuine recursive-descent parser with a full expression grammar
(logical OR/AND, equality, relational, additive, multiplicative, unary,
primary — correct operator precedence), not a token-skipper. It **builds
and returns an AST** representing the program's structure; pass `--tree`
on the command line to print it.

Malformed input raises a `ParseError` naming what was expected, what was
found, and the line number — see `input_error.txt` for a working demo
(missing semicolon, unclosed block).

## Structural Analysis

Walks the parse tree (not the raw token stream) to count:
- `if` / `else if` / `else` occurrences
- `for` and `while` loops
- Decision points (`if` + `else if` + `for` + `while`)
- Maximum block-nesting depth

## Complexity Calculation

```
Complexity Score = 1 + (decision points) + (nesting weight)
nesting weight    = max(0, maxNestingDepth - 1)
```

This is called **"Complexity Score"**, not "Cyclomatic Complexity" —
McCabe's cyclomatic complexity is a specific formula (`E - N + 2P`); this
project uses a simpler weighted score as originally defined in the term
project proposal, and the report labels it accordingly.

| Score Range | Classification |
| ----------- | -------------- |
| 1 – 5       | Low            |
| 6 – 15      | Medium         |
| Above 15    | High           |

---

## Compilation & Execution

```bash
g++ -std=c++11 lexer.cpp ast.cpp parser.cpp analyzer.cpp report.cpp main.cpp -o analyzer
```

```bash
./analyzer input.txt              # run the report
./analyzer input.txt --tree       # also print the parse tree
./analyzer input_complex.txt      # deeper-nesting sample
./analyzer input_error.txt        # demonstrates syntax-error reporting
```

On Windows, run `analyzer.exe` instead of `./analyzer`.

---

## Test Files Included

| File                    | Purpose                                                         |
| ------------------------ | ---------------------------------------------------------------- |
| `input.txt`              | Original sample program (matches earlier project drafts).       |
| `input_complex.txt`      | Deeper nesting, `else if`, nested `for`/`while` — reaches higher complexity. |
| `input_operators.txt`    | Exercises underscores and multi-character operators.            |
| `input_error.txt`        | Deliberately broken (missing `;` and `}`) to demo error handling. |

## Sample Output (`input.txt`)

```
--------- Structural Metrics ---------
If Statements          : 1
Else-If Branches       : 0
Else Statements        : 1
For Loops               : 1
While Loops             : 1
Decision Points         : 3
Maximum Nesting Depth   : 4

Complexity Score        : 7
Risk Classification     : Medium
```

## Sample Output (`input_error.txt`)

```
Syntax Error (line 11): Expected ';' after assignment to 'x' but found 'return'
```

---

## Known Limitations

- Single parameterless function per file; no function calls or parameters.
- No arrays, strings-as-values, or user-defined types.
- No semantic analysis (type checking) — only lexical, syntactic, and
  structural analysis are performed, matching the term project's proposed scope.
- Tested against four sample programs, not a full conformance suite.

---

## Future Enhancements

- Support multiple functions and function calls.
- Add basic type checking (semantic analysis phase).
- Export the parse tree and report as JSON/HTML for visualization.
- Add a graphical interface.

---

## Project Information

**Project Title:** Compiler-Based Code Complexity Analyzer
**Course:** Compiler Construction
**Project Type:** Individual Term Project
**Language:** C++
