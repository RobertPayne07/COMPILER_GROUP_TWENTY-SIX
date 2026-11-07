#include "ASTPrinter.h"

// PS/CSC/23/0303 - Jonathan Armah Mensah: ASTPrinter implementation

ASTPrinter::ASTPrinter(std::ostream& out)
    : out(out), indentLevel(0) {}

void ASTPrinter::printIndent() {
    for (int i = 0; i < indentLevel; ++i) {
        out << "  ";
    }
}

void ASTPrinter::print(const ProgramAST* program) {
    if (!program) {
        out << "(null program)\n";
        return;
    }
    out << "Program [line " << program->line << ", col " << program->column << "]\n";
    indentLevel++;
    for (const auto& decl : program->declarations) {
        printNode(decl.get());
    }
    indentLevel--;
}

void ASTPrinter::printNode(const ASTNode* node) {
    if (!node) return;

    if (auto func = dynamic_cast<const FunctionDeclAST*>(node)) {
        printIndent();
        out << "FunctionDecl: " << func->functionName << " -> " << func->returnType
            << " [line " << func->line << ", col " << func->column << "]\n";
        indentLevel++;
        if (!func->parameters.empty()) {
            printIndent();
            out << "Parameters:\n";
            indentLevel++;
            for (const auto& p : func->parameters) {
                printIndent();
                out << p->paramName << " : " << p->typeName << "\n";
            }
            indentLevel--;
        }
        if (func->body) {
            printStmt(func->body.get());
        }
        indentLevel--;
        return;
    }

    if (auto var = dynamic_cast<const VarDeclAST*>(node)) {
        printIndent();
        out << "VarDecl: " << var->varName << " : " << var->typeName
            << " [line " << var->line << ", col " << var->column << "]\n";
        if (var->initializer) {
            indentLevel++;
            printIndent();
            out << "Initializer:\n";
            indentLevel++;
            printExpr(var->initializer.get());
            indentLevel -= 2;
        }
        return;
    }

    if (auto stmt = dynamic_cast<const StmtAST*>(node)) {
        printStmt(stmt);
        return;
    }

    if (auto expr = dynamic_cast<const ExprAST*>(node)) {
        printExpr(expr);
        return;
    }
}

void ASTPrinter::printStmt(const StmtAST* stmt) {
    if (!stmt) return;

    if (auto block = dynamic_cast<const BlockStmtAST*>(stmt)) {
        printIndent();
        out << "BlockStmt [line " << block->line << ", col " << block->column << "]\n";
        indentLevel++;
        for (const auto& s : block->statements) {
            printStmt(s.get());
        }
        indentLevel--;
        return;
    }

    if (auto var = dynamic_cast<const VarDeclAST*>(stmt)) {
        printNode(var);
        return;
    }

    if (auto ifStmt = dynamic_cast<const IfStmtAST*>(stmt)) {
        printIndent();
        out << "IfStmt [line " << ifStmt->line << ", col " << ifStmt->column << "]\n";
        indentLevel++;
        printIndent();
        out << "Condition:\n";
        indentLevel++;
        printExpr(ifStmt->condition.get());
        indentLevel--;
        printIndent();
        out << "Then:\n";
        indentLevel++;
        printStmt(ifStmt->thenBranch.get());
        indentLevel--;
        if (ifStmt->elseBranch) {
            printIndent();
            out << "Else:\n";
            indentLevel++;
            printStmt(ifStmt->elseBranch.get());
            indentLevel--;
        }
        indentLevel--;
        return;
    }

    if (auto whileStmt = dynamic_cast<const WhileStmtAST*>(stmt)) {
        printIndent();
        out << "WhileStmt [line " << whileStmt->line << ", col " << whileStmt->column << "]\n";
        indentLevel++;
        printIndent();
        out << "Condition:\n";
        indentLevel++;
        printExpr(whileStmt->condition.get());
        indentLevel--;
        printIndent();
        out << "Body:\n";
        indentLevel++;
        printStmt(whileStmt->body.get());
        indentLevel -= 2;
        return;
    }

    if (auto forStmt = dynamic_cast<const ForStmtAST*>(stmt)) {
        printIndent();
        out << "ForStmt [line " << forStmt->line << ", col " << forStmt->column << "]\n";
        indentLevel++;
        if (forStmt->initializer) {
            printIndent();
            out << "Init:\n";
            indentLevel++;
            printStmt(forStmt->initializer.get());
            indentLevel--;
        }
        if (forStmt->condition) {
            printIndent();
            out << "Condition:\n";
            indentLevel++;
            printExpr(forStmt->condition.get());
            indentLevel--;
        }
        if (forStmt->increment) {
            printIndent();
            out << "Increment:\n";
            indentLevel++;
            printExpr(forStmt->increment.get());
            indentLevel--;
        }
        printIndent();
        out << "Body:\n";
        indentLevel++;
        printStmt(forStmt->body.get());
        indentLevel -= 2;
        return;
    }

    if (auto ret = dynamic_cast<const ReturnStmtAST*>(stmt)) {
        printIndent();
        out << "ReturnStmt [line " << ret->line << ", col " << ret->column << "]\n";
        if (ret->value) {
            indentLevel++;
            printExpr(ret->value.get());
            indentLevel--;
        }
        return;
    }

    if (auto brk = dynamic_cast<const BreakStmtAST*>(stmt)) {
        printIndent();
        out << "BreakStmt [line " << brk->line << ", col " << brk->column << "]\n";
        return;
    }

    if (auto cnt = dynamic_cast<const ContinueStmtAST*>(stmt)) {
        printIndent();
        out << "ContinueStmt [line " << cnt->line << ", col " << cnt->column << "]\n";
        return;
    }

    if (auto prt = dynamic_cast<const PrintStmtAST*>(stmt)) {
        printIndent();
        out << "PrintStmt [line " << prt->line << ", col " << prt->column << "]\n";
        indentLevel++;
        printExpr(prt->expression.get());
        indentLevel--;
        return;
    }

    if (auto rd = dynamic_cast<const ReadStmtAST*>(stmt)) {
        printIndent();
        out << "ReadStmt: " << rd->variableName << " [line " << rd->line << ", col " << rd->column << "]\n";
        return;
    }

    if (auto exprStmt = dynamic_cast<const ExprStmtAST*>(stmt)) {
        printIndent();
        out << "ExprStmt [line " << exprStmt->line << ", col " << exprStmt->column << "]\n";
        indentLevel++;
        printExpr(exprStmt->expression.get());
        indentLevel--;
        return;
    }
}

void ASTPrinter::printExpr(const ExprAST* expr) {
    if (!expr) return;

    if (auto lit = dynamic_cast<const LiteralExprAST*>(expr)) {
        printIndent();
        out << "Literal (" << lit->value << ") [line " << lit->line << ", col " << lit->column << "]\n";
        return;
    }

    if (auto var = dynamic_cast<const VariableExprAST*>(expr)) {
        printIndent();
        out << "Variable (" << var->name << ") [line " << var->line << ", col " << var->column << "]\n";
        return;
    }

    if (auto bin = dynamic_cast<const BinaryExprAST*>(expr)) {
        printIndent();
        out << "BinaryExpr (" << bin->opLexeme << ") [line " << bin->line << ", col " << bin->column << "]\n";
        indentLevel++;
        printExpr(bin->left.get());
        printExpr(bin->right.get());
        indentLevel--;
        return;
    }

    if (auto un = dynamic_cast<const UnaryExprAST*>(expr)) {
        printIndent();
        out << "UnaryExpr (" << un->opLexeme << ") [line " << un->line << ", col " << un->column << "]\n";
        indentLevel++;
        printExpr(un->operand.get());
        indentLevel--;
        return;
    }

    if (auto asgn = dynamic_cast<const AssignExprAST*>(expr)) {
        printIndent();
        out << "AssignExpr: " << asgn->targetName << " = [line " << asgn->line << ", col " << asgn->column << "]\n";
        indentLevel++;
        printExpr(asgn->value.get());
        indentLevel--;
        return;
    }

    if (auto call = dynamic_cast<const CallExprAST*>(expr)) {
        printIndent();
        out << "CallExpr: " << call->callee << "() [line " << call->line << ", col " << call->column << "]\n";
        indentLevel++;
        for (const auto& arg : call->arguments) {
            printExpr(arg.get());
        }
        indentLevel--;
        return;
    }
}
