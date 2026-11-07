@echo off
echo ========================================================
echo Building Compiler Project (Phase 1 Scanner + Phase 2 Parser)
echo ========================================================
if not exist bin mkdir bin

set SRCS=src/Token.cpp src/SourceBuffer.cpp src/ErrorHandler.cpp src/CommentFilter.cpp src/IdentifierScanner.cpp src/LiteralScanner.cpp src/OperatorScanner.cpp src/Scanner.cpp src/AST.cpp src/ParseErrorHandler.cpp src/ParserCore.cpp src/PrimaryExprParser.cpp src/BinaryExprParser.cpp src/ControlFlowParser.cpp src/DeclarationParser.cpp src/Parser.cpp src/ASTPrinter.cpp

echo Compiling compiler binary (bin/compiler.exe and bin/scanner.exe)...
g++ -std=c++17 -Wall -Iinclude %SRCS% src/main.cpp -o bin/compiler.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compiler build failed!
    exit /b 1
)
copy bin\compiler.exe bin\scanner.exe >nul 2>&1
copy bin\compiler.exe bin\parser.exe >nul 2>&1

echo Compiling Phase 1 Scanner unit tests (bin/test_scanner.exe)...
g++ -std=c++17 -Wall -Iinclude src/Token.cpp src/SourceBuffer.cpp src/ErrorHandler.cpp src/CommentFilter.cpp src/IdentifierScanner.cpp src/LiteralScanner.cpp src/OperatorScanner.cpp src/Scanner.cpp tests/test_scanner.cpp -o bin/test_scanner.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] test_scanner build failed!
    exit /b 1
)

echo Compiling Phase 2 Parser unit tests (bin/test_parser.exe)...
g++ -std=c++17 -Wall -Iinclude %SRCS% tests/test_parser.cpp -o bin/test_parser.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] test_parser build failed!
    exit /b 1
)

echo [SUCCESS] All binaries and test suites built successfully into bin/
echo Run: bin\compiler.exe examples\sample2_parser.min
echo Test: bin\test_parser.exe
