#include "PrimaryExprParser.h"

// PS/CSC/23/0294 - Jeremiah Adongo Ayinbila: PrimaryExprParser implementation

std::unique_ptr<ExprAST> PrimaryExprParser::parsePrimary(ParserCore& core, const ExprParserFn& parseExpr) {
    const Token& token = core.peek();

    // 1. Literal expressions (int, float, string, char, bool)
    if (token.type == TokenType::LITERAL_INT ||
        token.type == TokenType::LITERAL_FLOAT ||
        token.type == TokenType::LITERAL_STRING ||
        token.type == TokenType::LITERAL_CHAR ||
        token.type == TokenType::LITERAL_BOOL) {
        core.advance();
        return std::make_unique<LiteralExprAST>(token.type, token.lexeme, token.line, token.column);
    }

    // 2. Identifier: either a variable reference or a function call
    if (token.type == TokenType::IDENTIFIER) {
        Token idToken = core.advance();

        // Check if this is a function call: identifier '(' [args] ')'
        if (core.match(TokenType::DELIM_LPAREN)) {
            std::vector<std::unique_ptr<ExprAST>> args;
            if (!core.check(TokenType::DELIM_RPAREN)) {
                do {
                    args.push_back(parseExpr());
                } while (core.match(TokenType::DELIM_COMMA));
            }
            core.consume(TokenType::DELIM_RPAREN, "Expected ')' after function arguments.");
            return std::make_unique<CallExprAST>(idToken.lexeme, std::move(args), idToken.line, idToken.column);
        }

        return std::make_unique<VariableExprAST>(idToken.lexeme, idToken.line, idToken.column);
    }

    // 3. Parenthesized expression: '(' expr ')'
    if (core.match(TokenType::DELIM_LPAREN)) {
        int line = core.previous().line;
        int col = core.previous().column;
        auto expr = parseExpr();
        core.consume(TokenType::DELIM_RPAREN, "Expected ')' after grouped expression.");
        return expr;
    }

    // Fallback: report syntax error and advance to prevent infinite loop
    core.getErrorHandler().report(token, "Expected expression.");
    core.advance();
    return nullptr;
}

std::unique_ptr<ExprAST> PrimaryExprParser::parseUnary(ParserCore& core, const ExprParserFn& parseExpr) {
    // Unary operators: '!', '-', '+'
    if (core.match(TokenType::OP_LOGICAL_NOT) ||
        core.match(TokenType::OP_MINUS) ||
        core.match(TokenType::OP_PLUS)) {
        Token op = core.previous();
        auto operand = parseUnary(core, parseExpr);
        if (!operand) return nullptr;
        return std::make_unique<UnaryExprAST>(op.type, op.lexeme, std::move(operand), op.line, op.column);
    }

    return parsePrimary(core, parseExpr);
}
