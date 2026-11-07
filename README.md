# Compiler Construction Project — Phase 1 (Scanner) & Phase 2 (Parser)

**Group Name:** COMPILER_GROUP_ONE  
**Implementation Language:** C++17  
**Build System:** CMake / Makefile / build.bat  
**Phase 2 Deadline:** November 8, 2026 (Week 5)  

---

## Overview
This repository contains the full 5-phase compiler implementation for the class mini-language. 
- **Phase 1 (Completed):** Modular lexical analyzer (scanner) with full line/column coordinate tracking, whitespace/comment stripping, multi-character operator lookahead, and error diagnostics.
- **Phase 2 (Active):** Recursive-descent syntax analyzer (parser) with operator precedence climbing, panic-mode syntax error recovery, and Abstract Syntax Tree (AST) construction and visualization.

---

## Team Contributions & Commit Ownership (Phase 2: Parser)

| Index Number | Member Name | Branch | Assigned Component | Key Files |
| :--- | :--- | :--- | :--- | :--- |
| **PS/CSC/23/0302** | Payne Vincent Robert *(Repo Admin)* | `phase2/0302-cli-build` | CLI Integration & Build System | `src/main.cpp`, `CMakeLists.txt`, `Makefile`, `build.bat` |
| **PS/CSC/23/0291** | Ernest Amoako Agyemang | `phase2/0291-ast-model` | AST Base & Node Hierarchy | `include/AST.h`, `src/AST.cpp` |
| **PS/CSC/23/0292** | Yassin Alhassan | `phase2/0292-parser-core` | Parser Core & Token Stream | `include/ParserCore.h`, `src/ParserCore.cpp` |
| **PS/CSC/23/0293** | Mohammed Abdul Hamid | `phase2/0293-syntax-errors` | Syntax Diagnostics & Error Recovery | `include/ParseErrorHandler.h`, `src/ParseErrorHandler.cpp` |
| **PS/CSC/23/0294** | Jeremiah Adongo Ayinbila | `phase2/0294-primary-expr` | Primary & Unary Expression Parsing | `include/PrimaryExprParser.h`, `src/PrimaryExprParser.cpp` |
| **PS/CSC/23/0295** | Judah Fiifi Tawiah | `phase2/0295-binary-expr` | Binary Expressions & Operator Precedence | `include/BinaryExprParser.h`, `src/BinaryExprParser.cpp` |
| **PS/CSC/23/0297** | Isaac Seyram Benson | `phase2/0297-control-flow` | Control Flow Statements (If, While, For) | `include/ControlFlowParser.h`, `src/ControlFlowParser.cpp` |
| **PS/CSC/23/0299** | Kweku Andoh | `phase2/0299-declarations` | Variable & Function Declarations | `include/DeclarationParser.h`, `src/DeclarationParser.cpp` |
| **PS/CSC/23/0301** | Student 0301 | `phase2/0301-parser-orchestrator`| Parser Coordinator & Program Parser | `include/Parser.h`, `src/Parser.cpp` |
| **PS/CSC/23/0303** | Jonathan Armah Mensah | `phase2/0303-ast-tests-docs`| AST Visualizer, Tests & Docs | `include/ASTPrinter.h`, `src/ASTPrinter.cpp`, `tests/test_parser.cpp`, `examples/sample2_parser.min` |

---

## Building the Project

### Using Windows Batch Script (Recommended for Windows):
```cmd
build.bat
```
Binaries generated in `bin/`:
- `bin/compiler.exe` (main compiler executable)
- `bin/scanner.exe` (Phase 1 scanner)
- `bin/parser.exe` (Phase 2 parser)
- `bin/test_scanner.exe` (Phase 1 test suite)
- `bin/test_parser.exe` (Phase 2 test suite)

### Using Makefile (Linux / macOS / MinGW):
```bash
make
./bin/compiler examples/sample2_parser.min
./bin/test_parser
```

### Using CMake:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Running the Compiler

### Run the Parser (Phase 2 Default):
```bash
bin/compiler.exe examples/sample2_parser.min
```

### Run the Scanner Only (Phase 1):
```bash
bin/compiler.exe examples/sample1.min --scan
```

### Run the Test Suites:
```bash
bin/test_scanner.exe
bin/test_parser.exe
```

---

## Sample Parser Output (Abstract Syntax Tree)
```text
========================================================
               PARSER OUTPUT: ABSTRACT SYNTAX TREE      
========================================================
Program [line 4, col 1]
  VarDecl: globalCount : int [line 4, col 1]
    Initializer:
      Literal (0) [line 4, col 19]
  FunctionDecl: calculate -> int [line 6, col 1]
    Parameters:
      base : int
      multiplier : int
    BlockStmt [line 6, col 41]
      VarDecl: total : int [line 7, col 5]
        Initializer:
          Literal (0) [line 7, col 17]
      ForStmt [line 10, col 5]
        Init:
          ExprStmt [line 10, col 10]
            AssignExpr: i = [line 10, col 12]
              Literal (0) [line 10, col 14]
        Condition:
          BinaryExpr (<) [line 10, col 19]
            Variable (i) [line 10, col 17]
            Variable (multiplier) [line 10, col 21]
...
========================================================
Parsing completed successfully with 0 syntax errors!
========================================================
```
