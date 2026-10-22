#include "OperatorScanner.h"

// PS/CSC/23/0297 - Isaac Seyram Benson: OperatorScanner implementation

std::optional<Token> OperatorScanner::scan(SourceBuffer& buffer, int startLine, int startCol, ErrorHandler& errorHandler) {
    char c = buffer.advance();

    switch (c) {
        // Operators with potential 2-character sequences
        case '=':
            if (buffer.match('=')) return Token(TokenType::OP_EQUAL, "==", startLine, startCol);
            return Token(TokenType::OP_ASSIGN, "=", startLine, startCol);

        case '!':
            if (buffer.match('=')) return Token(TokenType::OP_NOT_EQUAL, "!=", startLine, startCol);
            return Token(TokenType::OP_LOGICAL_NOT, "!", startLine, startCol);

        case '<':
            if (buffer.match('=')) return Token(TokenType::OP_LESS_EQ, "<=", startLine, startCol);
            return Token(TokenType::OP_LESS, "<", startLine, startCol);

        case '>':
            if (buffer.match('=')) return Token(TokenType::OP_GREATER_EQ, ">=", startLine, startCol);
            return Token(TokenType::OP_GREATER, ">", startLine, startCol);

        case '&':
            if (buffer.match('&')) return Token(TokenType::OP_LOGICAL_AND, "&&", startLine, startCol);
            errorHandler.report(startLine, startCol, "Unexpected character '&', did you mean '&&'?");
            return Token(TokenType::TOKEN_ERROR, "&", startLine, startCol);

        case '|':
            if (buffer.match('|')) return Token(TokenType::OP_LOGICAL_OR, "||", startLine, startCol);
            errorHandler.report(startLine, startCol, "Unexpected character '|', did you mean '||'?");
            return Token(TokenType::TOKEN_ERROR, "|", startLine, startCol);

        // Simple single-character operators
        case '+': return Token(TokenType::OP_PLUS, "+", startLine, startCol);
        case '-': return Token(TokenType::OP_MINUS, "-", startLine, startCol);
        case '*': return Token(TokenType::OP_MULTIPLY, "*", startLine, startCol);
        case '/': return Token(TokenType::OP_DIVIDE, "/", startLine, startCol);
        case '%': return Token(TokenType::OP_MODULO, "%", startLine, startCol);

        // Delimiters
        case '(': return Token(TokenType::DELIM_LPAREN, "(", startLine, startCol);
        case ')': return Token(TokenType::DELIM_RPAREN, ")", startLine, startCol);
        case '{': return Token(TokenType::DELIM_LBRACE, "{", startLine, startCol);
        case '}': return Token(TokenType::DELIM_RBRACE, "}", startLine, startCol);
        case '[': return Token(TokenType::DELIM_LBRACKET, "[", startLine, startCol);
        case ']': return Token(TokenType::DELIM_RBRACKET, "]", startLine, startCol);
        case ';': return Token(TokenType::DELIM_SEMICOLON, ";", startLine, startCol);
        case ',': return Token(TokenType::DELIM_COMMA, ",", startLine, startCol);

        default:
            errorHandler.report(startLine, startCol, std::string("Unexpected character: '") + c + "'");
            return Token(TokenType::TOKEN_ERROR, std::string(1, c), startLine, startCol);
    }
}
