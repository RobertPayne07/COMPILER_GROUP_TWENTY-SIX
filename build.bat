@echo off
echo Building Compiler Scanner with g++...
if not exist bin mkdir bin

g++ -std=c++17 -Wall -Iinclude src/Token.cpp src/SourceBuffer.cpp src/ErrorHandler.cpp src/CommentFilter.cpp src/IdentifierScanner.cpp src/LiteralScanner.cpp src/OperatorScanner.cpp src/Scanner.cpp src/main.cpp -o bin/scanner.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed! Check compiler output.
    exit /b 1
)

g++ -std=c++17 -Wall -Iinclude src/Token.cpp src/SourceBuffer.cpp src/ErrorHandler.cpp src/CommentFilter.cpp src/IdentifierScanner.cpp src/LiteralScanner.cpp src/OperatorScanner.cpp src/Scanner.cpp tests/test_scanner.cpp -o bin/test_scanner.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Tests build failed!
    exit /b 1
)

echo [SUCCESS] Build complete! Executables located in bin/
