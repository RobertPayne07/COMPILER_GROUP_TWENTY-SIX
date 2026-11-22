#ifndef SEMANTIC_VALIDATOR_STMT_H
#define SEMANTIC_VALIDATOR_STMT_H

#include "AST.h"
#include "SymbolTable.h"
#include "SemanticErrorHandler.h"
#include <string>

// PS/CSC/23/0294 - Jeremiah Adongo Ayinbila: Statement and Control Flow Semantic Validator

class SemanticValidatorStmt {
private:
    int loopDepth;
    std::string currentFunctionReturnType;

public:
    SemanticValidatorStmt();

    void validateProgram(const ProgramAST* program, SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateFunctionDecl(const FunctionDeclAST* func, SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateStatement(const StmtAST* stmt, SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateBlock(const BlockStmtAST* block, SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateVarDecl(const VarDeclAST* var, SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateIf(const IfStmtAST* ifStmt, SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateWhile(const WhileStmtAST* whileStmt, SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateFor(const ForStmtAST* forStmt, SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateReturn(const ReturnStmtAST* ret, const SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateBreak(const BreakStmtAST* brk, SemanticErrorHandler& errHandler);
    void validateContinue(const ContinueStmtAST* cnt, SemanticErrorHandler& errHandler);
    void validatePrint(const PrintStmtAST* prt, const SymbolTable& symTab, SemanticErrorHandler& errHandler);
    void validateRead(const ReadStmtAST* rd, const SymbolTable& symTab, SemanticErrorHandler& errHandler);
};

#endif // SEMANTIC_VALIDATOR_STMT_H
