#include "ControlFlowParser.h"
#include "BinaryExprParser.h"

// PS/CSC/23/0297 - Isaac Seyram Benson: ControlFlowParser implementation

std::unique_ptr<StmtAST> ControlFlowParser::parseIf(ParserCore& core, const StmtParserFn& parseStmt) {
    Token ifToken = core.previous(); // 'if' was matched

    core.consume(TokenType::DELIM_LPAREN, "Expected '(' after 'if'.");
    auto condition = BinaryExprParser::parseExpression(core);
    core.consume(TokenType::DELIM_RPAREN, "Expected ')' after if condition.");

    auto thenBranch = parseStmt();
    std::unique_ptr<StmtAST> elseBranch = nullptr;

    if (core.match(TokenType::KEYWORD_ELSE)) {
        elseBranch = parseStmt();
    }

    return std::make_unique<IfStmtAST>(std::move(condition), std::move(thenBranch),
                                       std::move(elseBranch), ifToken.line, ifToken.column);
}

std::unique_ptr<StmtAST> ControlFlowParser::parseWhile(ParserCore& core, const StmtParserFn& parseStmt) {
    Token whileToken = core.previous(); // 'while' was matched

    core.consume(TokenType::DELIM_LPAREN, "Expected '(' after 'while'.");
    auto condition = BinaryExprParser::parseExpression(core);
    core.consume(TokenType::DELIM_RPAREN, "Expected ')' after while condition.");

    auto body = parseStmt();
    return std::make_unique<WhileStmtAST>(std::move(condition), std::move(body),
                                          whileToken.line, whileToken.column);
}

std::unique_ptr<StmtAST> ControlFlowParser::parseFor(ParserCore& core, const StmtParserFn& parseStmt) {
    Token forToken = core.previous(); // 'for' was matched

    core.consume(TokenType::DELIM_LPAREN, "Expected '(' after 'for'.");

    // 1. Initializer: can be ';' (empty), or an expression statement, etc.
    std::unique_ptr<StmtAST> initializer = nullptr;
    if (core.match(TokenType::DELIM_SEMICOLON)) {
        initializer = nullptr;
    } else {
        auto expr = BinaryExprParser::parseExpression(core);
        core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after for loop initializer.");
        initializer = std::make_unique<ExprStmtAST>(std::move(expr), forToken.line, forToken.column);
    }

    // 2. Condition: optional
    std::unique_ptr<ExprAST> condition = nullptr;
    if (!core.check(TokenType::DELIM_SEMICOLON)) {
        condition = BinaryExprParser::parseExpression(core);
    }
    core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after for loop condition.");

    // 3. Increment: optional
    std::unique_ptr<ExprAST> increment = nullptr;
    if (!core.check(TokenType::DELIM_RPAREN)) {
        increment = BinaryExprParser::parseExpression(core);
    }
    core.consume(TokenType::DELIM_RPAREN, "Expected ')' after for clauses.");

    // 4. Loop Body
    auto body = parseStmt();

    return std::make_unique<ForStmtAST>(std::move(initializer), std::move(condition),
                                        std::move(increment), std::move(body),
                                        forToken.line, forToken.column);
}

std::unique_ptr<StmtAST> ControlFlowParser::parseBreak(ParserCore& core) {
    Token token = core.previous();
    core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after 'break'.");
    return std::make_unique<BreakStmtAST>(token.line, token.column);
}

std::unique_ptr<StmtAST> ControlFlowParser::parseContinue(ParserCore& core) {
    Token token = core.previous();
    core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after 'continue'.");
    return std::make_unique<ContinueStmtAST>(token.line, token.column);
}
