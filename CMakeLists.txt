cmake_minimum_required(VERSION 3.15)
project(CompilerGroupOne VERSION 3.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Phase 1 Sources
set(SCANNER_SOURCES
    src/Token.cpp
    src/SourceBuffer.cpp
    src/ErrorHandler.cpp
    src/CommentFilter.cpp
    src/IdentifierScanner.cpp
    src/LiteralScanner.cpp
    src/OperatorScanner.cpp
    src/Scanner.cpp
)

# Phase 2 Sources
set(PARSER_SOURCES
    src/AST.cpp
    src/ParseErrorHandler.cpp
    src/ParserCore.cpp
    src/PrimaryExprParser.cpp
    src/BinaryExprParser.cpp
    src/ControlFlowParser.cpp
    src/DeclarationParser.cpp
    src/Parser.cpp
    src/ASTPrinter.cpp
)

# Phase 3 Sources
set(SEMANTIC_IR_SOURCES
    src/SymbolTable.cpp
    src/SemanticErrorHandler.cpp
    src/TypeCheckerExpr.cpp
    src/SemanticValidatorStmt.cpp
    src/IRInstruction.cpp
    src/IRGenExpr.cpp
    src/IRGenControlFlow.cpp
    src/IRProgram.cpp
)

set(ALL_SOURCES ${SCANNER_SOURCES} ${PARSER_SOURCES} ${SEMANTIC_IR_SOURCES})

# Executables
add_executable(compiler src/main.cpp ${ALL_SOURCES})
add_executable(scanner src/main.cpp ${ALL_SOURCES})
add_executable(parser src/main.cpp ${ALL_SOURCES})
add_executable(ir src/main.cpp ${ALL_SOURCES})

# Test Suites
add_executable(test_scanner tests/test_scanner.cpp ${SCANNER_SOURCES})
add_executable(test_parser tests/test_parser.cpp ${SCANNER_SOURCES} ${PARSER_SOURCES})
add_executable(test_ir tests/test_ir.cpp ${ALL_SOURCES})
