# Compiler Construction Project — Phase 1: Scanner

**Group Name:** COMPILER_GROUP_ONE  
**Implementation Language:** C++17  
**Build System:** CMake / Makefile / build.bat  

## Overview
This repository contains the full 5-phase compiler implementation for the class mini-language. Phase 1 implements a modular, high-performance lexical analyzer (scanner) with full line/column coordinate tracking, whitespace/comment stripping, multi-character operator lookahead, and error diagnostics.

## Team Contributions & Commit Ownership (Phase 1)

| Index Number | Member Name | Branch | Assigned Component | Key Files |
| :--- | :--- | :--- | :--- | :--- |
| **PS/CSC/23/0302** | Payne Vincent Robert *(Repo Admin)* | `phase1/0302-cli-build` | CLI Driver & Build System | `src/main.cpp`, `CMakeLists.txt`, `Makefile`, `build.bat` |
| **PS/CSC/23/0291** | Ernest Amoako Agyemang | `phase1/0291-token-model` | Token Types & Serialization | `include/Token.h`, `src/Token.cpp` |
| **PS/CSC/23/0292** | Yassin Alhassan | `phase1/0292-source-buffer` | File I/O & Coordinate Tracker | `include/SourceBuffer.h`, `src/SourceBuffer.cpp` |
| **PS/CSC/23/0293** | Mohammed Abdul Hamid | `phase1/0293-comment-filter` | Whitespace & Comment Stripper | `include/CommentFilter.h`, `src/CommentFilter.cpp` |
| **PS/CSC/23/0294** | Jeremiah Adongo Ayinbila | `phase1/0294-identifiers` | Keywords & Identifiers Scanner | `include/IdentifierScanner.h`, `src/IdentifierScanner.cpp` |
| **PS/CSC/23/0295** | Judah Fiifi Tawiah | `phase1/0295-literals` | Numeric & String/Char Literals | `include/LiteralScanner.h`, `src/LiteralScanner.cpp` |
| **PS/CSC/23/0297** | Isaac Seyram Benson | `phase1/0297-operators` | Operators & Delimiters | `include/OperatorScanner.h`, `src/OperatorScanner.cpp` |
| **PS/CSC/23/0299** | Kweku Andoh | `phase1/0299-error-handler` | Lexical Error Diagnostics | `include/ErrorHandler.h`, `src/ErrorHandler.cpp` |
| **PS/CSC/23/0301** | Student 0301 | `phase1/0301-scanner-core` | Scanner Coordinator Pipeline | `include/Scanner.h`, `src/Scanner.cpp` |
| **PS/CSC/23/0303** | Jonathan Armah Mensah | `phase1/0303-tests-docs` | Test Suite & Documentation | `tests/test_scanner.cpp`, `examples/*`, `README.md` |

## Building the Scanner

### Using CMake:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Using Makefile (Linux / macOS / MinGW):
```bash
make
./scanner examples/sample1.min
./test_scanner
```

### Using Windows Batch Script:
```cmd
build.bat
bin\scanner.exe examples\sample1.min
bin\test_scanner.exe
```

## Running the Scanner
```bash
./scanner <path_to_source_file>
```

Example output:
```text
========================================================
                  SCANNER OUTPUT REPORT                 
========================================================
Line  Col   Token Type          Lexeme
--------------------------------------------------------
5     1     KEYWORD_INT         int
5     5     IDENTIFIER          factorial
5     14    DELIM_LPAREN        (
5     15    KEYWORD_INT         int
5     19    IDENTIFIER          n
5     20    DELIM_RPAREN        )
5     22    DELIM_LBRACE        {
...
========================================================
Total Tokens: 84
Scanning completed successfully with 0 errors.
```
