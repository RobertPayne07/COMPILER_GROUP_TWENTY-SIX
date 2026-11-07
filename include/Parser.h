#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Token.h"
#include "ParserCore.h"
#include "ParseErrorHandler.h"
#include <vector>
#include <memory>
#include <string>

// PS/CSC/23/0301: Main Parser Pipeline Coordinator and Program Parser

class Parser {
private:
    ParserCore core;
    ParseErrorHandler errorHandler;

public:
    explicit Parser(std::vector<Token> tokens);
    static Parser fromTokens(std::vector<Token> tokens);
    static Parser fromSource(const std::string& sourceCode);
    static Parser fromFile(const std::string& filepath);

    std::unique_ptr<ProgramAST> parseProgram();
    std::unique_ptr<ASTNode> parseTopLevelDeclaration();
    std::unique_ptr<StmtAST> parseStatement();
    std::unique_ptr<BlockStmtAST> parseBlock();

    bool hasErrors() const { return errorHandler.hasErrors(); }
    const ParseErrorHandler& getErrorHandler() const { return errorHandler; }
};

#endif // PARSER_H
