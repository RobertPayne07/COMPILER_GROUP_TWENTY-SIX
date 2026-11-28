# Compiler Construction Project — Phase 1, Phase 2 & Phase 3

**Group Name:** COMPILER_GROUP_ONE  
**Implementation Language:** C++17  
**Build System:** CMake / Makefile / build.bat  
**Phase 3 Deadline:** November 22, 2026 (Week 6)  

---

## Overview
This repository contains the full 5-phase compiler implementation for the class mini-language. 
- **Phase 1 (Completed):** Modular lexical analyzer (scanner) with line/column tracking, comment stripping, and token generation.
- **Phase 2 (Completed):** Recursive-descent syntax analyzer (parser) with operator precedence climbing, panic-mode syntax recovery, and AST visualizer.
- **Phase 3 (Active):** Scoped Symbol Table, static type checking, control flow validation, and Three-Address Code (TAC) Intermediate Representation (IR) generation.

---

## Team Contributions & Commit Ownership (Phase 3: Semantic Analysis / IR)

| Index Number(Github Usernames) | Member Name | Branch | Assigned Component | Key Files |
| :--- | :--- | :--- | :--- | :--- |
| **PS/CSC/23/0302**(RobertPayne07) | Payne Vincent Robert *(Repo Admin)* | `phase3/0302-cli-build` | CLI Integration & Multi-Target Build | `src/main.cpp`, `CMakeLists.txt`, `Makefile`, `build.bat` |
| **PS/CSC/23/0291**(trapforever) | Ernest Amoako Agyemang | `phase3/0291-symbol-table` | Symbol Table & Scoping Architecture | `include/SymbolTable.h`, `src/SymbolTable.cpp` |
| **PS/CSC/23/0292**(yassinalhassan-pixel) | Yassin Alhassan | `phase3/0292-semantic-errors`| Semantic Diagnostics & Type Rules | `include/SemanticErrorHandler.h`, `src/SemanticErrorHandler.cpp` |
| **PS/CSC/23/0293**(Cypher563k) | Mohammed Abdul Hamid | `phase3/0293-type-checker` | Expression Type Checking & Validation | `include/TypeCheckerExpr.h`, `src/TypeCheckerExpr.cpp` |
| **PS/CSC/23/0294**(Jerryadongo) | Jeremiah Adongo Ayinbila | `phase3/0294-stmt-validator` | Statement & Control Flow Semantic Validation | `include/SemanticValidatorStmt.h`, `src/SemanticValidatorStmt.cpp` |
| **PS/CSC/23/0295**(judahtawiah6-eng) | Judah Fiifi Tawiah | `phase3/0295-ir-model` | Three-Address Code Architecture & Temps | `include/IRInstruction.h`, `src/IRInstruction.cpp` |
| **PS/CSC/23/0297**(bensonisaac001) | Isaac Seyram Benson | `phase3/0297-ir-expr` | Expression & Assignment IR Lowering | `include/IRGenExpr.h`, `src/IRGenExpr.cpp` |
| **PS/CSC/23/0299**(kwekuandoh2610-collab) | Kweku Andoh | `phase3/0299-ir-control-flow`| Control Flow, Loops & Jump IR Lowering | `include/IRGenControlFlow.h`, `src/IRGenControlFlow.cpp` |
| **PS/CSC/23/0301**(Abdul-Mujeeb1852) | Student 0301 | `phase3/0301-ir-coordinator` | IR Program Coordinator & Analyzer Pipeline | `include/IRProgram.h`, `src/IRProgram.cpp` |
| **PS/CSC/23/0303**(CyberGhost500) | Jonathan Armah Mensah | `phase3/0303-tests-docs` | Phase 3 Test Suite, Samples & Docs | `tests/test_ir.cpp`, `examples/sample3_ir.min`, `examples/sample3_semantic_errors.min` |

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
- `bin/ir.exe` (Phase 3 IR generator)
- `bin/test_scanner.exe` (Phase 1 test suite)
- `bin/test_parser.exe` (Phase 2 test suite)
- `bin/test_ir.exe` (Phase 3 test suite)

### Using Makefile (Linux / macOS / MinGW):
```bash
make
./bin/compiler examples/sample3_ir.min
./bin/test_ir
```

### Using CMake:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## Running the Compiler

### Run the IR Generator (Phase 3 Default):
```bash
bin/compiler.exe examples/sample3_ir.min
```

### Run Semantic Analysis Check Only:
```bash
bin/compiler.exe examples/sample3_ir.min --check
```

### Run Parser (Phase 2 AST):
```bash
bin/compiler.exe examples/sample2_parser.min --parse
```

### Run Scanner (Phase 1 Tokens):
```bash
bin/compiler.exe examples/sample1.min --scan
```

### Run All Test Suites:
```bash
bin/test_scanner.exe
bin/test_parser.exe
bin/test_ir.exe
```

---

## Sample Three-Address Code (TAC) Output
```text
========================================================
       PHASE 3: THREE-ADDRESS CODE (TAC) GENERATION     
========================================================

--- Generated Three-Address Code ---

  globalMultiplier = 2
func power:
  result = 1
  i = 0
L0:
  t0 = i < exp
  ifFalse t0 goto L2
  t1 = result * base
  result = t1
L1:
  t2 = i + 1
  i = t2
  goto L0
L2:
  return result
end_func power

func compute:
  ans = 0
  t3 = x > y
  ifFalse t3 goto L3
  param x
  param 2
  t4 = call power, 2
  t5 = t4 + y
  ans = t5
  goto L4
L3:
  param y
  param 2
  t6 = call power, 2
  t7 = t6 - x
  ans = t7
L4:
  t8 = ans * globalMultiplier
  ans = t8
  return ans
end_func compute
...
========================================================
Total IR Instructions: 47
IR Generation completed successfully with 0 errors!
========================================================
```
