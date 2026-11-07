#include "BinaryExprParser.h"
#include "PrimaryExprParser.h"

// PS/CSC/23/0295 - Judah Fiifi Tawiah: BinaryExprParser implementation

std::unique_ptr<ExprAST> BinaryExprParser::parseExpression(ParserCore& core) {
    return parseAssignment(core);
}

std::unique_ptr<ExprAST> BinaryExprParser::parseAssignment(ParserCore& core) {
    auto expr = parseLogicalOr(core);

    if (core.match(TokenType::OP_ASSIGN)) {
        Token equals = core.previous();
        auto value = parseAssignment(core); // Right-associative

        if (auto varExpr = dynamic_cast<VariableExprAST*>(expr.get())) {
            std::string name = varExpr->name;
            return std::make_unique<AssignExprAST>(name, std::move(value), equals.line, equals.column);
        }

        core.getErrorHandler().report(equals, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<ExprAST> BinaryExprParser::parseLogicalOr(ParserCore& core) {
    auto expr = parseLogicalAnd(core);

    while (core.match(TokenType::OP_LOGICAL_OR)) {
        Token op = core.previous();
        auto right = parseLogicalAnd(core);
        expr = std::make_unique<BinaryExprAST>(op.type, op.lexeme, std::move(expr), std::move(right), op.line, op.column);
    }

    return expr;
}

std::unique_ptr<ExprAST> BinaryExprParser::parseLogicalAnd(ParserCore& core) {
    auto expr = parseEquality(core);

    while (core.match(TokenType::OP_LOGICAL_AND)) {
        Token op = core.previous();
        auto right = parseEquality(core);
        expr = std::make_unique<BinaryExprAST>(op.type, op.lexeme, std::move(expr), std::move(right), op.line, op.column);
    }

    return expr;
}

std::unique_ptr<ExprAST> BinaryExprParser::parseEquality(ParserCore& core) {
    auto expr = parseComparison(core);

    while (core.match(TokenType::OP_EQUAL) || core.match(TokenType::OP_NOT_EQUAL)) {
        Token op = core.previous();
        auto right = parseComparison(core);
        expr = std::make_unique<BinaryExprAST>(op.type, op.lexeme, std::move(expr), std::move(right), op.line, op.column);
    }

    return expr;
}

std::unique_ptr<ExprAST> BinaryExprParser::parseComparison(ParserCore& core) {
    auto expr = parseTerm(core);

    while (core.match(TokenType::OP_GREATER) || core.match(TokenType::OP_GREATER_EQ) ||
           core.match(TokenType::OP_LESS) || core.match(TokenType::OP_LESS_EQ)) {
        Token op = core.previous();
        auto right = parseTerm(core);
        expr = std::make_unique<BinaryExprAST>(op.type, op.lexeme, std::move(expr), std::move(right), op.line, op.column);
    }

    return expr;
}

std::unique_ptr<ExprAST> BinaryExprParser::parseTerm(ParserCore& core) {
    auto expr = parseFactor(core);

    while (core.match(TokenType::OP_PLUS) || core.match(TokenType::OP_MINUS)) {
        Token op = core.previous();
        auto right = parseFactor(core);
        expr = std::make_unique<BinaryExprAST>(op.type, op.lexeme, std::move(expr), std::move(right), op.line, op.column);
    }

    return expr;
}

std::unique_ptr<ExprAST> BinaryExprParser::parseFactor(ParserCore& core) {
    auto expr = PrimaryExprParser::parseUnary(core, [&core]() {
        return BinaryExprParser::parseExpression(core);
    });

    while (core.match(TokenType::OP_MULTIPLY) || core.match(TokenType::OP_DIVIDE) || core.match(TokenType::OP_MODULO)) {
        Token op = core.previous();
        auto right = PrimaryExprParser::parseUnary(core, [&core]() {
            return BinaryExprParser::parseExpression(core);
        });
        expr = std::make_unique<BinaryExprAST>(op.type, op.lexeme, std::move(expr), std::move(right), op.line, op.column);
    }

    return expr;
}
