#ifndef TYPE_CHECKER_EXPR_H
#define TYPE_CHECKER_EXPR_H

#include "AST.h"
#include "SymbolTable.h"
#include "SemanticErrorHandler.h"
#include <string>

// PS/CSC/23/0293 - Mohammed Abdul Hamid: Expression Type Checking and Validation

class TypeCheckerExpr {
public:
    static std::string checkExpression(const ExprAST* expr,
                                       const SymbolTable& symTab,
                                       SemanticErrorHandler& errHandler);
};

#endif // TYPE_CHECKER_EXPR_H
