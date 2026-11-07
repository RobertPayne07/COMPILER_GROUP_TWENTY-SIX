#ifndef PARSER_CORE_H
#define PARSER_CORE_H

#include "Token.h"
#include "ParseErrorHandler.h"
#include <vector>
#include <string>

// PS/CSC/23/0292 - Yassin Alhassan: Parser Core and Token Stream Navigation

class ParserCore {
private:
    std::vector<Token> tokens;
    size_t current;
    ParseErrorHandler& errorHandler;

public:
    ParserCore(std::vector<Token> tokens, ParseErrorHandler& errorHandler);

    const Token& peek() const;
    const Token& peekNext() const;
    const Token& previous() const;
    bool isAtEnd() const;
    bool check(TokenType type) const;
    const Token& advance();
    bool match(TokenType type);
    bool matchAny(const std::initializer_list<TokenType>& types);
    const Token& consume(TokenType type, const std::string& message);

    void synchronize();
    size_t getCurrentIndex() const { return current; }
    ParseErrorHandler& getErrorHandler() { return errorHandler; }
};

#endif // PARSER_CORE_H
