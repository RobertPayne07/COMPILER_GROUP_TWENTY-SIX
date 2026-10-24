#include "Token.h"
#include <sstream>

// PS/CSC/23/0291 - Ernest Amoako Agyemang: Token string serialization and formatting

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KEYWORD_IF:        return "KEYWORD_IF";
        case TokenType::KEYWORD_ELSE:      return "KEYWORD_ELSE";
        case TokenType::KEYWORD_WHILE:     return "KEYWORD_WHILE";
        case TokenType::KEYWORD_FOR:       return "KEYWORD_FOR";
        case TokenType::KEYWORD_RETURN:    return "KEYWORD_RETURN";
        case TokenType::KEYWORD_INT:       return "KEYWORD_INT";
        case TokenType::KEYWORD_FLOAT:     return "KEYWORD_FLOAT";
        case TokenType::KEYWORD_BOOL:      return "KEYWORD_BOOL";
        case TokenType::KEYWORD_CHAR:      return "KEYWORD_CHAR";
        case TokenType::KEYWORD_VOID:      return "KEYWORD_VOID";
        case TokenType::KEYWORD_STRING:    return "KEYWORD_STRING";
        case TokenType::KEYWORD_PRINT:     return "KEYWORD_PRINT";
        case TokenType::KEYWORD_READ:      return "KEYWORD_READ";
        case TokenType::KEYWORD_BREAK:     return "KEYWORD_BREAK";
        case TokenType::KEYWORD_CONTINUE:  return "KEYWORD_CONTINUE";

        case TokenType::IDENTIFIER:        return "IDENTIFIER";
        case TokenType::LITERAL_INT:       return "LITERAL_INT";
        case TokenType::LITERAL_FLOAT:     return "LITERAL_FLOAT";
        case TokenType::LITERAL_STRING:    return "LITERAL_STRING";
        case TokenType::LITERAL_CHAR:      return "LITERAL_CHAR";
        case TokenType::LITERAL_BOOL:      return "LITERAL_BOOL";

        case TokenType::OP_PLUS:           return "OP_PLUS";
        case TokenType::OP_MINUS:          return "OP_MINUS";
        case TokenType::OP_MULTIPLY:       return "OP_MULTIPLY";
        case TokenType::OP_DIVIDE:         return "OP_DIVIDE";
        case TokenType::OP_MODULO:         return "OP_MODULO";

        case TokenType::OP_ASSIGN:         return "OP_ASSIGN";
        case TokenType::OP_EQUAL:          return "OP_EQUAL";
        case TokenType::OP_NOT_EQUAL:      return "OP_NOT_EQUAL";
        case TokenType::OP_LESS:           return "OP_LESS";
        case TokenType::OP_LESS_EQ:        return "OP_LESS_EQ";
        case TokenType::OP_GREATER:        return "OP_GREATER";
        case TokenType::OP_GREATER_EQ:     return "OP_GREATER_EQ";

        case TokenType::OP_LOGICAL_AND:    return "OP_LOGICAL_AND";
        case TokenType::OP_LOGICAL_OR:     return "OP_LOGICAL_OR";
        case TokenType::OP_LOGICAL_NOT:    return "OP_LOGICAL_NOT";

        case TokenType::DELIM_LPAREN:      return "DELIM_LPAREN";
        case TokenType::DELIM_RPAREN:      return "DELIM_RPAREN";
        case TokenType::DELIM_LBRACE:      return "DELIM_LBRACE";
        case TokenType::DELIM_RBRACE:      return "DELIM_RBRACE";
        case TokenType::DELIM_LBRACKET:    return "DELIM_LBRACKET";
        case TokenType::DELIM_RBRACKET:    return "DELIM_RBRACKET";
        case TokenType::DELIM_SEMICOLON:   return "DELIM_SEMICOLON";
        case TokenType::DELIM_COMMA:       return "DELIM_COMMA";

        case TokenType::TOKEN_EOF:         return "TOKEN_EOF";
        case TokenType::TOKEN_ERROR:       return "TOKEN_ERROR";
        default:                           return "UNKNOWN_TOKEN";
    }
}

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "Token(" << tokenTypeToString(type) 
        << ", \"" << lexeme << "\", line " << line 
        << ", col " << column << ")";
    return oss.str();
}
