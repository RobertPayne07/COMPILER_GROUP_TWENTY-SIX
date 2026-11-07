#include "Parser.h"
#include "Scanner.h"
#include "DeclarationParser.h"
#include "ControlFlowParser.h"
#include "BinaryExprParser.h"

// PS/CSC/23/0301: Parser coordinator implementation

Parser::Parser(std::vector<Token> tokens)
    : core(std::move(tokens), errorHandler) {}

Parser Parser::fromTokens(std::vector<Token> tokens) {
    return Parser(std::move(tokens));
}

Parser Parser::fromSource(const std::string& sourceCode) {
    Scanner scanner(sourceCode);
    std::vector<Token> tokens = scanner.scanTokens();
    return Parser(std::move(tokens));
}

Parser Parser::fromFile(const std::string& filepath) {
    Scanner scanner = Scanner::fromFile(filepath);
    std::vector<Token> tokens = scanner.scanTokens();
    return Parser(std::move(tokens));
}

std::unique_ptr<ProgramAST> Parser::parseProgram() {
    int startLine = core.peek().line;
    int startCol = core.peek().column;
    std::vector<std::unique_ptr<ASTNode>> decls;

    while (!core.isAtEnd()) {
        try {
            auto decl = parseTopLevelDeclaration();
            if (decl) {
                decls.push_back(std::move(decl));
            }
        } catch (...) {
            core.synchronize();
        }
    }

    return std::make_unique<ProgramAST>(std::move(decls), startLine, startCol);
}

std::unique_ptr<ASTNode> Parser::parseTopLevelDeclaration() {
    // If a type specifier is seen, check if next tokens indicate a function or global variable
    if (DeclarationParser::isTypeSpecifier(core.peek().type)) {
        // Lookahead: type IDENTIFIER '(' -> function
        if (core.peekNext().type == TokenType::IDENTIFIER) {
            size_t idx = core.getCurrentIndex();
            // Peek at token after identifier
            // We can safely check if the token after identifier is '('
            // Using DeclarationParser
            if (core.getCurrentIndex() + 2 < 1000000) { // bounds guard
                // Look ahead 2 tokens: type (current), identifier (current+1), '(' (current+2)
                // Let's check:
                Token id = core.peekNext();
                // We advance temporarily to inspect or look at ParserCore
            }
        }

        // Differentiate function vs var decl:
        // A function signature has '(' after the identifier name.
        if (core.peekNext().type == TokenType::IDENTIFIER) {
            // Check token after identifier
            // We can peek directly
            // Core allows checking peekNext(); let's see if we can check 2 ahead
            // In DeclarationParser, if type is matched and identifier is matched:
            // If next is '(', it's a function!
            TokenType typeTok = core.peek().type;
            // Let's do:
            std::string tName = core.advance().lexeme;
            Token idTok = core.consume(TokenType::IDENTIFIER, "Expected identifier after type.");

            if (core.check(TokenType::DELIM_LPAREN)) {
                // Function declaration:
                auto params = DeclarationParser::parseParameters(core);
                core.consume(TokenType::DELIM_LBRACE, "Expected '{' to begin function body.");
                auto body = parseBlock();
                return std::make_unique<FunctionDeclAST>(tName, idTok.lexeme, std::move(params),
                                                        std::move(body), idTok.line, idTok.column);
            } else {
                // Variable declaration:
                std::unique_ptr<ExprAST> init = nullptr;
                if (core.match(TokenType::OP_ASSIGN)) {
                    init = BinaryExprParser::parseExpression(core);
                }
                core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after variable declaration.");
                return std::make_unique<VarDeclAST>(tName, idTok.lexeme, std::move(init), idTok.line, idTok.column);
            }
        }
    }

    // If unexpected top-level token, report and synchronize
    errorHandler.report(core.peek(), "Unexpected token in top-level declaration.");
    core.synchronize();
    return nullptr;
}

std::unique_ptr<BlockStmtAST> Parser::parseBlock() {
    int line = core.previous().line; // '{' was consumed
    int col = core.previous().column;
    std::vector<std::unique_ptr<StmtAST>> stmts;

    while (!core.check(TokenType::DELIM_RBRACE) && !core.isAtEnd()) {
        try {
            auto stmt = parseStatement();
            if (stmt) {
                stmts.push_back(std::move(stmt));
            }
        } catch (...) {
            core.synchronize();
        }
    }

    core.consume(TokenType::DELIM_RBRACE, "Expected '}' after block.");
    return std::make_unique<BlockStmtAST>(std::move(stmts), line, col);
}

std::unique_ptr<StmtAST> Parser::parseStatement() {
    // 1. Block statement: '{' ... '}'
    if (core.match(TokenType::DELIM_LBRACE)) {
        return parseBlock();
    }

    // 2. Control flow statements
    if (core.match(TokenType::KEYWORD_IF)) {
        return ControlFlowParser::parseIf(core, [this]() { return parseStatement(); });
    }
    if (core.match(TokenType::KEYWORD_WHILE)) {
        return ControlFlowParser::parseWhile(core, [this]() { return parseStatement(); });
    }
    if (core.match(TokenType::KEYWORD_FOR)) {
        return ControlFlowParser::parseFor(core, [this]() { return parseStatement(); });
    }
    if (core.match(TokenType::KEYWORD_BREAK)) {
        return ControlFlowParser::parseBreak(core);
    }
    if (core.match(TokenType::KEYWORD_CONTINUE)) {
        return ControlFlowParser::parseContinue(core);
    }

    // 3. Return statement
    if (core.match(TokenType::KEYWORD_RETURN)) {
        Token retTok = core.previous();
        std::unique_ptr<ExprAST> val = nullptr;
        if (!core.check(TokenType::DELIM_SEMICOLON)) {
            val = BinaryExprParser::parseExpression(core);
        }
        core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after return value.");
        return std::make_unique<ReturnStmtAST>(std::move(val), retTok.line, retTok.column);
    }

    // 4. Print statement: print(expr);
    if (core.match(TokenType::KEYWORD_PRINT)) {
        Token printTok = core.previous();
        core.consume(TokenType::DELIM_LPAREN, "Expected '(' after 'print'.");
        auto expr = BinaryExprParser::parseExpression(core);
        core.consume(TokenType::DELIM_RPAREN, "Expected ')' after print argument.");
        core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after print statement.");
        return std::make_unique<PrintStmtAST>(std::move(expr), printTok.line, printTok.column);
    }

    // 5. Read statement: read(id);
    if (core.match(TokenType::KEYWORD_READ)) {
        Token readTok = core.previous();
        core.consume(TokenType::DELIM_LPAREN, "Expected '(' after 'read'.");
        Token varTok = core.consume(TokenType::IDENTIFIER, "Expected variable name in read statement.");
        core.consume(TokenType::DELIM_RPAREN, "Expected ')' after variable name.");
        core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after read statement.");
        return std::make_unique<ReadStmtAST>(varTok.lexeme, readTok.line, readTok.column);
    }

    // 6. Local variable declaration: int x = 5;
    if (DeclarationParser::isTypeSpecifier(core.peek().type)) {
        return DeclarationParser::parseVarDecl(core);
    }

    // 7. Expression statement: expr;
    int line = core.peek().line;
    int col = core.peek().column;
    auto expr = BinaryExprParser::parseExpression(core);
    core.consume(TokenType::DELIM_SEMICOLON, "Expected ';' after expression.");
    return std::make_unique<ExprStmtAST>(std::move(expr), line, col);
}
