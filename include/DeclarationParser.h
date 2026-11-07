#ifndef DECLARATION_PARSER_H
#define DECLARATION_PARSER_H

#include "AST.h"
#include "ParserCore.h"
#include <memory>
#include <functional>

// PS/CSC/23/0299 - Kweku Andoh: Declaration Parsing (Variable Declarations, Functions, and Parameters)

class DeclarationParser {
public:
    using StmtParserFn = std::function<std::unique_ptr<StmtAST>()>;

    static bool isTypeSpecifier(TokenType type);
    static std::string parseType(ParserCore& core);
    static std::unique_ptr<VarDeclAST> parseVarDecl(ParserCore& core);
    static std::unique_ptr<FunctionDeclAST> parseFunctionDecl(ParserCore& core, const StmtParserFn& parseStmt);
    static std::vector<std::unique_ptr<ParamDeclAST>> parseParameters(ParserCore& core);
};

#endif // DECLARATION_PARSER_H
