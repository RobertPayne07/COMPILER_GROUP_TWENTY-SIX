#include "ParserCore.h"

// PS/CSC/23/0292 - Yassin Alhassan: ParserCore implementation and token movement

ParserCore::ParserCore(std::vector<Token> tokens, ParseErrorHandler& errorHandler)
    : tokens(std::move(tokens)), current(0), errorHandler(errorHandler) {}

const Token& ParserCore::peek() const {
    if (current >= tokens.size()) {
        static Token eofToken(TokenType::TOKEN_EOF, "", 0, 0);
        return eofToken;
    }
    return tokens[current];
}

const Token& ParserCore::peekNext() const {
    if (current + 1 >= tokens.size()) {
        static Token eofToken(TokenType::TOKEN_EOF, "", 0, 0);
        return eofToken;
    }
    return tokens[current + 1];
}

const Token& ParserCore::previous() const {
    if (current == 0) {
        return tokens[0];
    }
    return tokens[current - 1];
}

bool ParserCore::isAtEnd() const {
    return peek().type == TokenType::TOKEN_EOF;
}

bool ParserCore::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

const Token& ParserCore::advance() {
    if (!isAtEnd()) {
        current++;
    }
    return previous();
}

bool ParserCore::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool ParserCore::matchAny(const std::initializer_list<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

const Token& ParserCore::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        return advance();
    }
    errorHandler.report(peek(), message);
    return peek();
}

void ParserCore::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::DELIM_SEMICOLON) {
            return;
        }

        switch (peek().type) {
            case TokenType::KEYWORD_IF:
            case TokenType::KEYWORD_WHILE:
            case TokenType::KEYWORD_FOR:
            case TokenType::KEYWORD_RETURN:
            case TokenType::KEYWORD_INT:
            case TokenType::KEYWORD_FLOAT:
            case TokenType::KEYWORD_BOOL:
            case TokenType::KEYWORD_CHAR:
            case TokenType::KEYWORD_VOID:
            case TokenType::KEYWORD_STRING:
            case TokenType::KEYWORD_PRINT:
            case TokenType::KEYWORD_READ:
                return;
            default:
                break;
        }

        advance();
    }
}
