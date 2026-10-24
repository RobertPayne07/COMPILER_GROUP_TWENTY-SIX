#include "LiteralScanner.h"
#include <cctype>

// PS/CSC/23/0295 - Judah Fiifi Tawiah: LiteralScanner implementation

Token LiteralScanner::scanNumber(SourceBuffer& buffer, int startLine, int startCol) {
    std::string lexeme;
    bool isFloat = false;

    while (!buffer.isAtEnd() && std::isdigit(static_cast<unsigned char>(buffer.peek()))) {
        lexeme += buffer.advance();
    }

    // Check for fractional part
    if (!buffer.isAtEnd() && buffer.peek() == '.' && std::isdigit(static_cast<unsigned char>(buffer.peekNext()))) {
        isFloat = true;
        lexeme += buffer.advance(); // consume '.'

        while (!buffer.isAtEnd() && std::isdigit(static_cast<unsigned char>(buffer.peek()))) {
            lexeme += buffer.advance();
        }
    }

    TokenType type = isFloat ? TokenType::LITERAL_FLOAT : TokenType::LITERAL_INT;
    return Token(type, lexeme, startLine, startCol);
}

Token LiteralScanner::scanString(SourceBuffer& buffer, int startLine, int startCol, ErrorHandler& errorHandler) {
    std::string lexeme = "\"";
    buffer.advance(); // consume opening quote '"'

    bool closed = false;
    while (!buffer.isAtEnd()) {
        char c = buffer.peek();
        if (c == '\n') {
            // Strings in our mini language cannot span newlines without escape
            break;
        }

        if (c == '"') {
            lexeme += buffer.advance(); // consume closing quote
            closed = true;
            break;
        }

        if (c == '\\') {
            lexeme += buffer.advance(); // consume '\'
            if (!buffer.isAtEnd()) {
                lexeme += buffer.advance(); // consume escaped char
            }
            continue;
        }

        lexeme += buffer.advance();
    }

    if (!closed) {
        errorHandler.report(startLine, startCol, "Unterminated string literal.");
        return Token(TokenType::TOKEN_ERROR, lexeme, startLine, startCol);
    }

    return Token(TokenType::LITERAL_STRING, lexeme, startLine, startCol);
}

Token LiteralScanner::scanChar(SourceBuffer& buffer, int startLine, int startCol, ErrorHandler& errorHandler) {
    std::string lexeme = "'";
    buffer.advance(); // consume opening quote '\''

    bool closed = false;
    if (!buffer.isAtEnd() && buffer.peek() != '\'') {
        if (buffer.peek() == '\\') {
            lexeme += buffer.advance(); // consume '\'
            if (!buffer.isAtEnd()) {
                lexeme += buffer.advance();
            }
        } else {
            lexeme += buffer.advance();
        }
    }

    if (!buffer.isAtEnd() && buffer.peek() == '\'') {
        lexeme += buffer.advance(); // consume closing quote
        closed = true;
    }

    if (!closed) {
        errorHandler.report(startLine, startCol, "Unterminated character literal.");
        return Token(TokenType::TOKEN_ERROR, lexeme, startLine, startCol);
    }

    return Token(TokenType::LITERAL_CHAR, lexeme, startLine, startCol);
}
