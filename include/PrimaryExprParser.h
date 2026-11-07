#ifndef PRIMARY_EXPR_PARSER_H
#define PRIMARY_EXPR_PARSER_H

#include "AST.h"
#include "ParserCore.h"
#include <memory>
#include <functional>

// PS/CSC/23/0294 - Jeremiah Adongo Ayinbila: Primary and Unary Expression Parsing

class PrimaryExprParser {
public:
    using ExprParserFn = std::function<std::unique_ptr<ExprAST>()>;

    static std::unique_ptr<ExprAST> parsePrimary(ParserCore& core, const ExprParserFn& parseExpr);
    static std::unique_ptr<ExprAST> parseUnary(ParserCore& core, const ExprParserFn& parseExpr);
};

#endif // PRIMARY_EXPR_PARSER_H
