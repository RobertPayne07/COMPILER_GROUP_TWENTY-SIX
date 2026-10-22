#include "Scanner.h"
#include <iostream>
#include <iomanip>

// PS/CSC/23/0302 - Payne Vincent Robert: Main CLI Entry Point & Command Line Arguments

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " <source_file> [--verbose]\n";
    std::cout << "Options:\n";
    std::cout << "  --verbose    Print detailed token metadata\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string filepath = argv[1];
    bool verbose = false;
    if (argc >= 3 && std::string(argv[2]) == "--verbose") {
        verbose = true;
    }

    try {
        Scanner scanner = Scanner::fromFile(filepath);
        std::vector<Token> tokens = scanner.scanTokens();

        std::cout << "===\n";
        std::cout << "                  SCANNER OUTPUT REPORT                 \n";
        std::cout << "===\n";
        std::cout << std::left 
                  << std::setw(6)  << "Line" 
                  << std::setw(6)  << "Col" 
                  << std::setw(20) << "Token Type" 
                  << "Lexeme\n";
        std::cout << "---\n";

        for (const auto& token : tokens) {
            std::cout << std::left 
                      << std::setw(6)  << token.line 
                      << std::setw(6)  << token.column 
                      << std::setw(20) << tokenTypeToString(token.type) 
                      << token.lexeme << "\n";
        }
        std::cout << "====\n";
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
