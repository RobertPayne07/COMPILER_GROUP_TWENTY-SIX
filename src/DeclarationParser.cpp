#include "DeclarationParser.h"
#include "BinaryExprParser.h"

// PS/CSC/23/0299 - Kweku Andoh: DeclarationParser implementation

bool DeclarationParser::isTypeSpecifier(TokenType type) {
    return type == TokenType::KEYWORD_INT ||
           type == TokenType::KEYWORD_FLOAT ||
           type == TokenType::KEYWORD_BOOL ||
           type == TokenType::KEYWORD_CHAR ||
           type == TokenType::KEYWORD_VOID ||
           type == TokenType::KEYWORD_STRING;
}

std::string DeclarationParser::parseType(ParserCore& core) {
    if (isTypeSpecifier(core.peek().type)) {
        return core.advance().lexeme;
    }
    core.getErrorHandler().report(core.peek(), "Expected type specifier (int, float, bool, char, void, string).");
    return "unknown_type";
}

std::unique_ptr<VarDeclAST> DeclarationParser::parseVarDecl(ParserCore& core) {
    int line = core.peek().line;
    int col = core.peek().column;

    std::string typeName = parseType(core);
    Token idToken = core.consume(TokenType::IDENTIFIER, "Expected variable name after type specifier.");

    std::unique_ptr<ExprAST> initializer = nullptr;
    if (core.match(TokenType::OP_ASSIGN)) {
        initializer = BinaryExprParser::parseExpression(core);
    }

    core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_unique<VarDeclAST>(typeName, idToken.lexeme, std::move(initializer), line, col);
}

std::vector<std::unique_ptr<ParamDeclAST>> DeclarationParser::parseParameters(ParserCore& core) {
    std::vector<std::unique_ptr<ParamDeclAST>> params;

    core.consume(TokenType::DELIM_LPAREN, "Expected '(' for function parameter list.");

    if (!core.check(TokenType::DELIM_RPAREN)) {
        do {
            int line = core.peek().line;
            int col = core.peek().column;
            std::string pType = parseType(core);
            Token pName = core.consume(TokenType::IDENTIFIER, "Expected parameter name.");
            params.push_back(std::make_unique<ParamDeclAST>(pType, pName.lexeme, line, col));
        } while (core.match(TokenType::DELIM_COMMA));
    }

    core.consume(TokenType::DELIM_RPAREN, "Expected ')' after function parameter list.");
    return params;
}

std::unique_ptr<FunctionDeclAST> DeclarationParser::parseFunctionDecl(ParserCore& core, const StmtParserFn& parseStmt) {
    int line = core.peek().line;
    int col = core.peek().column;

    std::string returnType = parseType(core);
    Token funcName = core.consume(TokenType::IDENTIFIER, "Expected function name.");

    auto params = parseParameters(core);

    // Body of function must be a block statement '{ ... }'
    std::unique_ptr<BlockStmtAST> body = nullptr;
    if (core.check(TokenType::DELIM_LBRACE)) {
        auto stmt = parseStmt();
        if (auto b = dynamic_cast<BlockStmtAST*>(stmt.get())) {
            stmt.release();
            body = std::unique_ptr<BlockStmtAST>(b);
        } else {
            core.getErrorHandler().report(funcName, "Expected block body '{ ... }' for function.");
        }
    } else {
        core.getErrorHandler().report(core.peek(), "Expected '{' to begin function body.");
    }

    return std::make_unique<FunctionDeclAST>(returnType, funcName.lexeme,
                                            std::move(params), std::move(body), line, col);
}
