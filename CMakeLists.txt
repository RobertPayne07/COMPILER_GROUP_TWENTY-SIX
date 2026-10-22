cmake_minimum_required(VERSION 3.15)
project(CompilerGroupOne VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/Token.cpp
    src/SourceBuffer.cpp
    src/ErrorHandler.cpp
    src/CommentFilter.cpp
    src/IdentifierScanner.cpp
    src/LiteralScanner.cpp
    src/OperatorScanner.cpp
    src/Scanner.cpp
)

# Compiler executable
add_executable(scanner src/main.cpp ${SOURCES})

# Unit tests executable
add_executable(test_scanner tests/test_scanner.cpp ${SOURCES})
