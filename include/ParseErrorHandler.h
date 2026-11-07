#ifndef PARSE_ERROR_HANDLER_H
#define PARSE_ERROR_HANDLER_H

#include "Token.h"
#include <string>
#include <vector>
#include <iostream>

// PS/CSC/23/0293 - Mohammed Abdul Hamid: Parser Error Handling and Syntax Diagnostics

struct ParseError {
    int line;
    int column;
    std::string message;
    std::string tokenLexeme;

    ParseError(int line, int column, std::string message, std::string tokenLexeme)
        : line(line), column(column), message(std::move(message)), tokenLexeme(std::move(tokenLexeme)) {}
};

class ParseErrorHandler {
private:
    std::vector<ParseError> errors;

public:
    void report(const Token& token, const std::string& message);
    void report(int line, int column, const std::string& message);
    bool hasErrors() const;
    const std::vector<ParseError>& getErrors() const;
    void printAll(std::ostream& out = std::cerr) const;
    void clear();
};

#endif // PARSE_ERROR_HANDLER_H
