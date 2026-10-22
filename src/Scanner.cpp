#include "Scanner.h"
#include <cctype>

// PS/CSC/23/0301: Scanner implementation & dispatch loop

Scanner::Scanner(std::string sourceCode)
    : buffer(std::move(sourceCode)) {}

Scanner Scanner::fromFile(const std::string& filepath) {
    return Scanner(SourceBuffer::fromFile(filepath).getSource());
}

Token Scanner::nextToken() {
    // 1. Skip whitespace and comments
    CommentFilter::skipWhitespaceAndComments(buffer, errorHandler);

    // 2. Check for EOF
    if (buffer.isAtEnd()) {
        return Token(TokenType::TOKEN_EOF, "", buffer.getLine(), buffer.getColumn());
    }

    int startLine = buffer.getLine();
    int startCol = buffer.getColumn();
    char c = buffer.peek();

    // 3. Identifiers & Keywords
    if (identifierScanner.isIdentifierStart(c)) {
        return identifierScanner.scan(buffer, startLine, startCol);
    }

    // 4. Numeric Literals
    if (std::isdigit(static_cast<unsigned char>(c))) {
        return LiteralScanner::scanNumber(buffer, startLine, startCol);
    }

    // 5. String Literals
    if (c == '"') {
        return LiteralScanner::scanString(buffer, startLine, startCol, errorHandler);
    }

    // 6. Character Literals
    if (c == '\'') {
        return LiteralScanner::scanChar(buffer, startLine, startCol, errorHandler);
    }

    // 7. Operators & Delimiters
    auto opToken = OperatorScanner::scan(buffer, startLine, startCol, errorHandler);
    if (opToken.has_value()) {
        return opToken.value();
    }

    // Fallback for unknown character (OperatorScanner handles error report)
    return Token(TokenType::TOKEN_ERROR, std::string(1, c), startLine, startCol);
}

std::vector<Token> Scanner::scanTokens() {
    std::vector<Token> tokens;
    while (true) {
        Token token = nextToken();
        tokens.push_back(token);
        if (token.type == TokenType::TOKEN_EOF) {
            break;
        }
    }
    return tokens;
}
