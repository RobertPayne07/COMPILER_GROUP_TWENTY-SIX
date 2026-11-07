#ifndef BINARY_EXPR_PARSER_H
#define BINARY_EXPR_PARSER_H

#include "AST.h"
#include "ParserCore.h"
#include <memory>

// PS/CSC/23/0295 - Judah Fiifi Tawiah: Binary Expression Parsing and Operator Precedence

class BinaryExprParser {
public:
    static std::unique_ptr<ExprAST> parseExpression(ParserCore& core);
    static std::unique_ptr<ExprAST> parseAssignment(ParserCore& core);
    static std::unique_ptr<ExprAST> parseLogicalOr(ParserCore& core);
    static std::unique_ptr<ExprAST> parseLogicalAnd(ParserCore& core);
    static std::unique_ptr<ExprAST> parseEquality(ParserCore& core);
    static std::unique_ptr<ExprAST> parseComparison(ParserCore& core);
    static std::unique_ptr<ExprAST> parseTerm(ParserCore& core);
    static std::unique_ptr<ExprAST> parseFactor(ParserCore& core);
};

#endif // BINARY_EXPR_PARSER_H
