#include "SemanticValidatorStmt.h"
#include "TypeCheckerExpr.h"

// PS/CSC/23/0294 - Jeremiah Adongo Ayinbila: SemanticValidatorStmt
// Implementation

SemanticValidatorStmt::SemanticValidatorStmt()
    : loopDepth(0), currentFunctionReturnType("") {}

void SemanticValidatorStmt::validateProgram(const ProgramAST *program,
                                            SymbolTable &symTab,
                                            SemanticErrorHandler &errHandler) {
  if (!program)
    return;

  // First pass: register function signatures in global scope
  for (const auto &decl : program->declarations) {
    if (auto func = dynamic_cast<const FunctionDeclAST *>(decl.get())) {
      std::vector<std::string> paramTypes;
      for (const auto &p : func->parameters) {
        paramTypes.push_back(p->typeName);
      }
      Symbol funcSym(func->functionName, func->returnType, SymbolKind::FUNCTION,
                     func->line, func->column, paramTypes);
      if (!symTab.define(funcSym)) {
        errHandler.report(func->line, func->column,
                          "Redefinition of function '" + func->functionName +
                              "'.");
      }
    } else if (auto var = dynamic_cast<const VarDeclAST *>(decl.get())) {
      validateVarDecl(var, symTab, errHandler);
    }
  }

  // Second pass: validate function bodies
  for (const auto &decl : program->declarations) {
    if (auto func = dynamic_cast<const FunctionDeclAST *>(decl.get())) {
      validateFunctionDecl(func, symTab, errHandler);
    }
  }
}

void SemanticValidatorStmt::validateFunctionDecl(
    const FunctionDeclAST *func, SymbolTable &symTab,
    SemanticErrorHandler &errHandler) {
  currentFunctionReturnType = func->returnType;

  symTab.enterScope();

  // Define parameters in function scope
  for (const auto &p : func->parameters) {
    Symbol paramSym(p->paramName, p->typeName, SymbolKind::PARAMETER, p->line,
                    p->column);
    if (!symTab.define(paramSym)) {
      errHandler.report(p->line, p->column,
                        "Duplicate parameter name '" + p->paramName +
                            "' in function '" + func->functionName + "'.");
    }
  }

  if (func->body) {
    for (const auto &stmt : func->body->statements) {
      validateStatement(stmt.get(), symTab, errHandler);
    }
  }

  symTab.exitScope();
  currentFunctionReturnType = "";
}

void SemanticValidatorStmt::validateStatement(
    const StmtAST *stmt, SymbolTable &symTab,
    SemanticErrorHandler &errHandler) {
  if (!stmt)
    return;

  if (auto block = dynamic_cast<const BlockStmtAST *>(stmt)) {
    validateBlock(block, symTab, errHandler);
    return;
  }
  if (auto var = dynamic_cast<const VarDeclAST *>(stmt)) {
    validateVarDecl(var, symTab, errHandler);
    return;
  }
  if (auto ifStmt = dynamic_cast<const IfStmtAST *>(stmt)) {
    validateIf(ifStmt, symTab, errHandler);
    return;
  }
  if (auto whileStmt = dynamic_cast<const WhileStmtAST *>(stmt)) {
    validateWhile(whileStmt, symTab, errHandler);
    return;
  }
  if (auto forStmt = dynamic_cast<const ForStmtAST *>(stmt)) {
    validateFor(forStmt, symTab, errHandler);
    return;
  }
  if (auto ret = dynamic_cast<const ReturnStmtAST *>(stmt)) {
    validateReturn(ret, symTab, errHandler);
    return;
  }
  if (auto brk = dynamic_cast<const BreakStmtAST *>(stmt)) {
    validateBreak(brk, errHandler);
    return;
  }
  if (auto cnt = dynamic_cast<const ContinueStmtAST *>(stmt)) {
    validateContinue(cnt, errHandler);
    return;
  }
  if (auto prt = dynamic_cast<const PrintStmtAST *>(stmt)) {
    validatePrint(prt, symTab, errHandler);
    return;
  }
  if (auto rd = dynamic_cast<const ReadStmtAST *>(stmt)) {
    validateRead(rd, symTab, errHandler);
    return;
  }
  if (auto exprStmt = dynamic_cast<const ExprStmtAST *>(stmt)) {
    TypeCheckerExpr::checkExpression(exprStmt->expression.get(), symTab,
                                     errHandler);
    return;
  }
}

void SemanticValidatorStmt::validateBlock(const BlockStmtAST *block,
                                          SymbolTable &symTab,
                                          SemanticErrorHandler &errHandler) {
  symTab.enterScope();
  for (const auto &s : block->statements) {
    validateStatement(s.get(), symTab, errHandler);
  }
  symTab.exitScope();
}

void SemanticValidatorStmt::validateVarDecl(const VarDeclAST *var,
                                            SymbolTable &symTab,
                                            SemanticErrorHandler &errHandler) {
  Symbol sym(var->varName, var->typeName, SymbolKind::VARIABLE, var->line,
             var->column);
  if (!symTab.define(sym)) {
    errHandler.report(var->line, var->column,
                      "Redeclaration of variable '" + var->varName +
                          "' in the same scope.");
  }

  if (var->initializer) {
    std::string initType = TypeCheckerExpr::checkExpression(
        var->initializer.get(), symTab, errHandler);
    if (initType != "error_type" &&
        !TypeUtils::areTypesCompatible(var->typeName, initType)) {
      errHandler.report(var->line, var->column,
                        "Type mismatch in initializer for '" + var->varName +
                            "'. Declared as '" + var->typeName +
                            "' but initialized with '" + initType + "'.");
    }
  }
}

void SemanticValidatorStmt::validateIf(const IfStmtAST *ifStmt,
                                       SymbolTable &symTab,
                                       SemanticErrorHandler &errHandler) {
  std::string condType = TypeCheckerExpr::checkExpression(
      ifStmt->condition.get(), symTab, errHandler);
  if (condType != "error_type" && condType != "bool" && condType != "int") {
    errHandler.report(
        ifStmt->line, ifStmt->column,
        "If condition must evaluate to boolean or integer, found '" + condType +
            "'.");
  }

  validateStatement(ifStmt->thenBranch.get(), symTab, errHandler);
  if (ifStmt->elseBranch) {
    validateStatement(ifStmt->elseBranch.get(), symTab, errHandler);
  }
}

void SemanticValidatorStmt::validateWhile(const WhileStmtAST *whileStmt,
                                          SymbolTable &symTab,
                                          SemanticErrorHandler &errHandler) {
  std::string condType = TypeCheckerExpr::checkExpression(
      whileStmt->condition.get(), symTab, errHandler);
  if (condType != "error_type" && condType != "bool" && condType != "int") {
    errHandler.report(
        whileStmt->line, whileStmt->column,
        "While condition must evaluate to boolean or integer, found '" +
            condType + "'.");
  }

  loopDepth++;
  validateStatement(whileStmt->body.get(), symTab, errHandler);
  loopDepth--;
}

void SemanticValidatorStmt::validateFor(const ForStmtAST *forStmt,
                                        SymbolTable &symTab,
                                        SemanticErrorHandler &errHandler) {
  symTab.enterScope();

  if (forStmt->initializer) {
    validateStatement(forStmt->initializer.get(), symTab, errHandler);
  }
  if (forStmt->condition) {
    std::string condType = TypeCheckerExpr::checkExpression(
        forStmt->condition.get(), symTab, errHandler);
    if (condType != "error_type" && condType != "bool" && condType != "int") {
      errHandler.report(
          forStmt->line, forStmt->column,
          "For loop condition must evaluate to boolean or integer, found '" +
              condType + "'.");
    }
  }
  if (forStmt->increment) {
    TypeCheckerExpr::checkExpression(forStmt->increment.get(), symTab,
                                     errHandler);
  }

  loopDepth++;
  validateStatement(forStmt->body.get(), symTab, errHandler);
  loopDepth--;

  symTab.exitScope();
}

void SemanticValidatorStmt::validateReturn(const ReturnStmtAST *ret,
                                           const SymbolTable &symTab,
                                           SemanticErrorHandler &errHandler) {
  std::string actualType = "void";
  if (ret->value) {
    actualType =
        TypeCheckerExpr::checkExpression(ret->value.get(), symTab, errHandler);
  }

  if (currentFunctionReturnType.empty()) {
    errHandler.report(ret->line, ret->column,
                      "Return statement outside of function.");
    return;
  }

  if (!TypeUtils::areTypesCompatible(currentFunctionReturnType, actualType)) {
    errHandler.report(ret->line, ret->column,
                      "Function return type mismatch. Expected '" +
                          currentFunctionReturnType + "', but returned '" +
                          actualType + "'.");
  }
}

void SemanticValidatorStmt::validateBreak(const BreakStmtAST *brk,
                                          SemanticErrorHandler &errHandler) {
  if (loopDepth == 0) {
    errHandler.report(brk->line, brk->column,
                      "'break' statement not within a loop.");
  }
}

void SemanticValidatorStmt::validateContinue(const ContinueStmtAST *cnt,
                                             SemanticErrorHandler &errHandler) {
  if (loopDepth == 0) {
    errHandler.report(cnt->line, cnt->column,
                      "'continue' statement not within a loop.");
  }
}

void SemanticValidatorStmt::validatePrint(const PrintStmtAST *prt,
                                          const SymbolTable &symTab,
                                          SemanticErrorHandler &errHandler) {
  std::string exprType = TypeCheckerExpr::checkExpression(prt->expression.get(),
                                                          symTab, errHandler);
  if (exprType == "void") {
    errHandler.report(prt->line, prt->column, "Cannot print void expression.");
  }
}

void SemanticValidatorStmt::validateRead(const ReadStmtAST *rd,
                                         const SymbolTable &symTab,
                                         SemanticErrorHandler &errHandler) {
  const Symbol *sym = symTab.lookup(rd->variableName);
  if (!sym) {
    errHandler.report(rd->line, rd->column,
                      "Cannot read into undeclared variable '" +
                          rd->variableName + "'.");
  }
}
