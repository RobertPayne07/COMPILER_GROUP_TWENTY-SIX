#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "AST.h"
#include <string>
#include <iostream>

// PS/CSC/23/0303 - Jonathan Armah Mensah: AST Visualizer and Tree Pretty-Printer

class ASTPrinter {
private:
    std::ostream& out;
    int indentLevel;

    void printIndent();
    void printNode(const ASTNode* node);
    void printExpr(const ExprAST* expr);
    void printStmt(const StmtAST* stmt);

public:
    explicit ASTPrinter(std::ostream& out = std::cout);
    void print(const ProgramAST* program);
};

#endif // AST_PRINTER_H
