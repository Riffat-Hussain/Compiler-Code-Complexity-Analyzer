<h1 align="center">Compiler-Based Code Complexity Analyzer</h1>

<p align="center">
A Compiler Construction Term Project implemented in <b>C++</b> that performs lexical analysis, recursive-descent parsing, parse tree construction, structural analysis, and complexity calculation for a simplified C-like language.
</p>

<p align="center">

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue)
![Compiler Construction](https://img.shields.io/badge/Course-Compiler%20Construction-green)
![Project](https://img.shields.io/badge/Type-Term%20Project-orange)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)

</p>

---

# 📑 Table of Contents

* 📌 Project Overview
* 🎯 Project Objectives
* ✨ Features
* 🏗️ System Architecture
* 📂 Project Structure
* 🔍 Lexical Analysis
* 🌳 Syntax Analysis
* 📊 Structural Analysis
* 📈 Complexity Calculation
* ⚙️ Compilation & Execution
* 🧪 Test Files Included
* 💻 Sample Output
* ⚠️ Known Limitations
* 🚀 Future Enhancements
* 🛠 Technologies Used
* 👨‍💻 Project Information

---

# 📌 Project Overview

The **Compiler-Based Code Complexity Analyzer** is an individual **Compiler Construction Term Project** developed in **C++**.

Unlike a traditional compiler that translates source code into machine code, this project focuses entirely on the **front-end phases of compilation**. It performs lexical analysis, syntax analysis, parse tree construction, and structural analysis to evaluate the complexity of programs written in a simplified C-like language.

Instead of generating executable code, the analyzer produces a detailed complexity report describing the structural characteristics of the source program.

**Supported Language Features**

* Single parameterless function definition
* Variable declarations
* Arithmetic and logical expressions
* Assignment operators (`=`, `+=`, `-=`, `*=`, `/=`)
* Increment and decrement operators
* `if`
* `else if`
* `else`
* `while`
* `for`
* `return`

---

# 🎯 Project Objectives

* Apply compiler construction concepts in a practical implementation.
* Perform lexical analysis and generate a complete token stream.
* Implement a recursive-descent parser that constructs a real parse tree (AST).
* Detect syntax errors with meaningful messages and line numbers.
* Analyze program structure using the generated parse tree.
* Calculate a structural complexity score.
* Generate a readable complexity report.

---

# ✨ Features

* ✅ Hand-written Lexical Analyzer
* ✅ Recursive Descent Parser
* ✅ Abstract Syntax Tree (AST)
* ✅ Structural Complexity Analysis
* ✅ Complexity Score Calculation
* ✅ Parse Tree Visualization
* ✅ Syntax Error Reporting
* ✅ Multiple Sample Input Programs
* ✅ Modular Project Architecture
* ✅ Easy to Extend

---

# 🏗️ System Architecture

```text
             Source Code
                  │
                  ▼
         Lexical Analyzer
                  │
                  ▼
            Token Stream
                  │
                  ▼
    Recursive Descent Parser
                  │
                  ▼
      Abstract Syntax Tree
                  │
                  ▼
      Structural Analyzer
                  │
                  ▼
        Complexity Report
```

---

# 📂 Project Structure

| File              | Responsibility                                                        |
| ----------------- | --------------------------------------------------------------------- |
| `lexer.h/.cpp`    | Converts source code into tokens while tracking line numbers.         |
| `ast.h/.cpp`      | Defines Abstract Syntax Tree nodes and tree printer.                  |
| `parser.h/.cpp`   | Implements a recursive-descent parser that constructs the AST.        |
| `analyzer.h/.cpp` | Traverses the AST to compute structural metrics and complexity score. |
| `report.h/.cpp`   | Formats and prints the final complexity report.                       |
| `main.cpp`        | Coordinates all compiler phases and handles errors.                   |

---

# 🔍 Lexical Analysis

The lexical analyzer scans the input source code character by character and converts it into a stream of tokens.

### Supported Token Types

* KEYWORD
* IDENTIFIER
* NUMBER
* OPERATOR
* DELIMITER
* STRING
* EOF_TOKEN

### Additional Features

* Identifiers containing underscores
* Integer and floating-point numbers
* Multi-character operators

```
== != <= >= && || ++ -- += -= *= /=
```

* Line comments

```cpp
// comment
```

* Block comments

```cpp
/* comment */
```

Any invalid character immediately produces a **LexError** together with its corresponding line number.

---

# 🌳 Syntax Analysis

The parser is implemented using the **Recursive Descent Parsing** technique.

It performs:

* Recursive parsing
* Operator precedence parsing
* Parse tree construction
* Syntax validation
* Error reporting

Rather than simply validating tokens, the parser constructs an **Abstract Syntax Tree (AST)** representing the structure of the program.

When an invalid program is encountered, the parser throws a **ParseError** containing:

* Expected symbol
* Actual symbol
* Source line number

The parse tree can also be displayed using:

```bash
./analyzer input.txt --tree
```

---

# 📊 Structural Analysis

Instead of analyzing raw tokens, the analyzer traverses the generated AST to compute program complexity.

The following metrics are collected:

* Number of `if` statements
* Number of `else if` branches
* Number of `else` branches
* Number of `for` loops
* Number of `while` loops
* Decision points
* Maximum nesting depth

These values are later used to compute the overall complexity score.

---

# 📈 Complexity Calculation

The project calculates complexity using the following formula:

```text
Complexity Score =
1 + Decision Points + Nesting Weight
```

Where

```text
Nesting Weight =
max(0, Maximum Nesting Depth − 1)
```

### Complexity Classification

| Score    | Classification |
| -------- | -------------- |
| 1 – 5    | Low            |
| 6 – 15   | Medium         |
| Above 15 | High           |

**Note:** This project intentionally uses the term **Complexity Score** rather than **Cyclomatic Complexity**, since it follows the scoring approach proposed for the term project rather than McCabe's original metric.

---

# ⚙️ Compilation & Execution

Compile

```bash
g++ -std=c++11 lexer.cpp ast.cpp parser.cpp analyzer.cpp report.cpp main.cpp -o analyzer
```

Run

```bash
./analyzer input.txt
```

Print Parse Tree

```bash
./analyzer input.txt --tree
```

Run Complex Sample

```bash
./analyzer input_complex.txt
```

Test Syntax Error Handling

```bash
./analyzer input_error.txt
```

For Windows:

```bash
analyzer.exe input.txt
```

---

# 🧪 Test Files Included

| File                  | Purpose                                                 |
| --------------------- | ------------------------------------------------------- |
| `input.txt`           | Basic sample program.                                   |
| `input_complex.txt`   | Demonstrates nested loops and multiple decision points. |
| `input_operators.txt` | Tests identifiers and multi-character operators.        |
| `input_error.txt`     | Demonstrates syntax error detection.                    |

---

# 💻 Sample Output

### Structural Metrics

```text
--------- Structural Metrics ---------

If Statements          : 1
Else-If Branches       : 0
Else Statements        : 1
For Loops              : 1
While Loops            : 1
Decision Points        : 3
Maximum Nesting Depth  : 4

Complexity Score       : 7
Risk Classification    : Medium
```

### Syntax Error Example

```text
Syntax Error (line 11):
Expected ';' after assignment to 'x'
but found 'return'
```

---

# ⚠️ Known Limitations

* Supports only one parameterless function.
* No user-defined functions.
* No arrays.
* No semantic analysis (type checking).
* Tested using multiple sample programs rather than a complete language test suite.

---

# 🚀 Future Enhancements

* Support multiple functions.
* Function calls and parameters.
* Semantic analysis.
* Type checking.
* Symbol table implementation.
* JSON report export.
* HTML report generation.
* Graphical User Interface (GUI).

---

# 🛠 Technologies Used

* C++
* Object-Oriented Programming
* Recursive Descent Parsing
* Abstract Syntax Trees (AST)
* Compiler Construction Concepts
* Visual Studio Code
* Git
* GitHub

---

# 👨‍💻 Project Information

| Item                 | Details                                 |
| -------------------- | --------------------------------------- |
| **Project**          | Compiler-Based Code Complexity Analyzer |
| **Course**           | Compiler Construction                   |
| **Language**         | C++                                     |
| **Project Type**     | Individual Term Project                 |
| **University**       | Karakoram International University      |
| **Student**          | Rifat Hussain                           |
| **Registration No.** | 2023-KIU-BS4019                         |

---

## ⭐ Acknowledgement

This project was developed as part of the **Compiler Construction** course to demonstrate the practical implementation of compiler front-end concepts, including lexical analysis, parsing, Abstract Syntax Tree construction, and structural code complexity analysis.

---

## 📄 License

This repository is intended for **academic and educational purposes only**.

---


**Project Type:** Individual Term Project
**Language:** C++
