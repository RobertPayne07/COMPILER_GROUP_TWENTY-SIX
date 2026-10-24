#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

// PS/CSC/23/0291 - Ernest Amoako Agyemang: Token Definitions and Types

enum class TokenType {
    // Keywords
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_WHILE,
    KEYWORD_FOR,
    KEYWORD_RETURN,
    KEYWORD_INT,
    KEYWORD_FLOAT,
    KEYWORD_BOOL,
    KEYWORD_CHAR,
    KEYWORD_VOID,
    KEYWORD_STRING,
    KEYWORD_PRINT,
    KEYWORD_READ,
    KEYWORD_BREAK,
    KEYWORD_CONTINUE,

    // Identifiers & Literals
    IDENTIFIER,
    LITERAL_INT,
    LITERAL_FLOAT,
    LITERAL_STRING,
    LITERAL_CHAR,
    LITERAL_BOOL,

    // Arithmetic Operators
    OP_PLUS,        // +
    OP_MINUS,       // -
    OP_MULTIPLY,    // *
    OP_DIVIDE,      // /
    OP_MODULO,      // %

    // Assignment & Relational Operators
    OP_ASSIGN,      // =
    OP_EQUAL,       // ==
    OP_NOT_EQUAL,   // !=
    OP_LESS,        // <
    OP_LESS_EQ,     // <=
    OP_GREATER,     // >
    OP_GREATER_EQ,  // >=

    // Logical Operators
    OP_LOGICAL_AND, // &&
    OP_LOGICAL_OR,  // ||
    OP_LOGICAL_NOT, // !

    // Delimiters & Punctuation
    DELIM_LPAREN,   // (
    DELIM_RPAREN,   // )
    DELIM_LBRACE,   // {
    DELIM_RBRACE,   // }
    DELIM_LBRACKET, // [
    DELIM_RBRACKET, // ]
    DELIM_SEMICOLON,// ;
    DELIM_COMMA,    // ,

    // Special
    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    Token(TokenType type, std::string lexeme, int line, int column)
        : type(type), lexeme(std::move(lexeme)), line(line), column(column) {}

    std::string toString() const;
};

std::string tokenTypeToString(TokenType type);

#endif // TOKEN_H
