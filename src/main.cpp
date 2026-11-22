#include "Scanner.h"
#include "Parser.h"
#include "ASTPrinter.h"
#include "IRProgram.h"
#include <iostream>
#include <iomanip>
#include <string>

// PS/CSC/23/0302 - Payne Vincent Robert: Main Compiler CLI Driver (Phases 1, 2, and 3)

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " <source_file> [options]\n";
    std::cout << "Options:\n";
    std::cout << "  --scan     Run Phase 1 Scanner only (token stream)\n";
    std::cout << "  --parse    Run Phase 2 Parser (Abstract Syntax Tree)\n";
    std::cout << "  --check    Run Phase 3 Semantic Analysis (type & scope checks)\n";
    std::cout << "  --ir       Run Phase 3 IR Generator (Three-Address Code) [default]\n";
    std::cout << "  --help     Display this help message\n";
}

int runScanner(const std::string& filepath) {
    try {
        Scanner scanner = Scanner::fromFile(filepath);
        std::vector<Token> tokens = scanner.scanTokens();

        std::cout << "========================================================\n";
        std::cout << "                  SCANNER OUTPUT REPORT                 \n";
        std::cout << "========================================================\n";
        std::cout << std::left 
                  << std::setw(6)  << "Line" 
                  << std::setw(6)  << "Col" 
                  << std::setw(20) << "Token Type" 
                  << "Lexeme\n";
        std::cout << "--------------------------------------------------------\n";

        for (const auto& token : tokens) {
            std::cout << std::left 
                      << std::setw(6)  << token.line 
                      << std::setw(6)  << token.column 
                      << std::setw(20) << tokenTypeToString(token.type) 
                      << token.lexeme << "\n";
        }
        std::cout << "========================================================\n";
        std::cout << "Total Tokens: " << tokens.size() << "\n";

        if (scanner.hasErrors()) {
            std::cout << "\nErrors encountered during scanning:\n";
            scanner.getErrorHandler().printAll(std::cerr);
            return 1;
        }

        std::cout << "Scanning completed successfully with 0 errors.\n";
        return 0;

    } catch (const std::exception& ex) {
        std::cerr << "Fatal Error: " << ex.what() << "\n";
        return 1;
    }
}

int runParser(const std::string& filepath) {
    try {
        std::cout << "========================================================\n";
        std::cout << "               PARSER OUTPUT: ABSTRACT SYNTAX TREE      \n";
        std::cout << "========================================================\n";

        Parser parser = Parser::fromFile(filepath);
        auto program = parser.parseProgram();

        if (parser.hasErrors()) {
            std::cout << "\nSyntax errors encountered during parsing:\n";
            parser.getErrorHandler().printAll(std::cerr);
            return 1;
        }

        ASTPrinter printer(std::cout);
        printer.print(program.get());

        std::cout << "========================================================\n";
        std::cout << "Parsing completed successfully with 0 syntax errors!\n";
        std::cout << "========================================================\n";
        return 0;

    } catch (const std::exception& ex) {
        std::cerr << "Fatal Error: " << ex.what() << "\n";
        return 1;
    }
}

int runSemanticCheck(const std::string& filepath) {
    try {
        std::cout << "========================================================\n";
        std::cout << "             PHASE 3: SEMANTIC ANALYSIS REPORT          \n";
        std::cout << "========================================================\n";

        Parser parser = Parser::fromFile(filepath);
        auto program = parser.parseProgram();

        if (parser.hasErrors()) {
            std::cout << "\nSyntax errors detected; cannot perform semantic analysis:\n";
            parser.getErrorHandler().printAll(std::cerr);
            return 1;
        }

        SemanticAnalyzer analyzer;
        bool success = analyzer.analyze(program.get());

        if (!success || analyzer.hasErrors()) {
            std::cout << "\nSemantic errors detected:\n";
            analyzer.getErrorHandler().printAll(std::cerr);
            return 1;
        }

        std::cout << "Semantic analysis passed with 0 errors!\n";
        std::cout << "  - All variable scopes resolved.\n";
        std::cout << "  - All types checked and compatible.\n";
        std::cout << "  - All loop controls (break/continue) verified.\n";
        std::cout << "========================================================\n";
        return 0;

    } catch (const std::exception& ex) {
        std::cerr << "Fatal Error: " << ex.what() << "\n";
        return 1;
    }
}

int runIRGenerator(const std::string& filepath) {
    try {
        std::cout << "========================================================\n";
        std::cout << "       PHASE 3: THREE-ADDRESS CODE (TAC) GENERATION     \n";
        std::cout << "========================================================\n";

        Parser parser = Parser::fromFile(filepath);
        auto program = parser.parseProgram();

        if (parser.hasErrors()) {
            std::cout << "\nSyntax errors detected:\n";
            parser.getErrorHandler().printAll(std::cerr);
            return 1;
        }

        SemanticAnalyzer analyzer;
        if (!analyzer.analyze(program.get())) {
            std::cout << "\nSemantic errors detected; aborting IR generation:\n";
            analyzer.getErrorHandler().printAll(std::cerr);
            return 1;
        }

        IRProgram ir = IRGenerator::generate(program.get());
        std::cout << "\n--- Generated Three-Address Code ---\n\n";
        ir.dump(std::cout);

        std::cout << "========================================================\n";
        std::cout << "Total IR Instructions: " << ir.getInstructions().size() << "\n";
        std::cout << "IR Generation completed successfully with 0 errors!\n";
        std::cout << "========================================================\n";
        return 0;

    } catch (const std::exception& ex) {
        std::cerr << "Fatal Error: " << ex.what() << "\n";
        return 1;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string arg1 = argv[1];
    if (arg1 == "--help" || arg1 == "-h") {
        printUsage(argv[0]);
        return 0;
    }

    std::string filepath = arg1;
    std::string mode = "--ir";

    if (argc >= 3) {
        mode = argv[2];
    }

    if (mode == "--scan") {
        return runScanner(filepath);
    } else if (mode == "--parse") {
        return runParser(filepath);
    } else if (mode == "--check") {
        return runSemanticCheck(filepath);
    } else {
        return runIRGenerator(filepath);
    }
}
