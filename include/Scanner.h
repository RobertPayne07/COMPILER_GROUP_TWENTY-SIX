#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"
#include "SourceBuffer.h"
#include "CommentFilter.h"
#include "IdentifierScanner.h"
#include "LiteralScanner.h"
#include "OperatorScanner.h"
#include "ErrorHandler.h"
#include <vector>

// PS/CSC/23/0301: Core Scanner Pipeline Coordination

class Scanner {
private:
    SourceBuffer buffer;
    ErrorHandler errorHandler;
    IdentifierScanner identifierScanner;

public:
    explicit Scanner(std::string sourceCode);
    static Scanner fromFile(const std::string& filepath);

    Token nextToken();
    std::vector<Token> scanTokens();
    const ErrorHandler& getErrorHandler() const { return errorHandler; }
    bool hasErrors() const { return errorHandler.hasErrors(); }
};

#endif // SCANNER_H
