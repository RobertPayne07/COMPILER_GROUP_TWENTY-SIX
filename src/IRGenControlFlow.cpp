#include "IRGenControlFlow.h"
#include "IRGenExpr.h"

// PS/CSC/23/0299 - Kweku Andoh: IRGenControlFlow Implementation

void IRGenControlFlow::generateStmt(const StmtAST* stmt,
                                   std::vector<IRInstruction>& instructions,
                                   TempGenerator& tempGen,
                                   std::vector<LoopContext>& loopStack) {
    if (!stmt) return;

    // 1. Block statement
    if (auto block = dynamic_cast<const BlockStmtAST*>(stmt)) {
        for (const auto& s : block->statements) {
            generateStmt(s.get(), instructions, tempGen, loopStack);
        }
        return;
    }

    // 2. Variable declaration
    if (auto var = dynamic_cast<const VarDeclAST*>(stmt)) {
        if (var->initializer) {
            std::string val = IRGenExpr::generateExpr(var->initializer.get(), instructions, tempGen);
            instructions.emplace_back(IROp::ASSIGN, var->varName, val, "", var->line);
        }
        return;
    }

    // 3. Expression statement
    if (auto exprStmt = dynamic_cast<const ExprStmtAST*>(stmt)) {
        IRGenExpr::generateExpr(exprStmt->expression.get(), instructions, tempGen);
        return;
    }

    // 4. If statement
    if (auto ifStmt = dynamic_cast<const IfStmtAST*>(stmt)) {
        std::string cond = IRGenExpr::generateExpr(ifStmt->condition.get(), instructions, tempGen);
        std::string elseLabel = tempGen.newLabel();
        std::string endLabel = tempGen.newLabel();

        instructions.emplace_back(IROp::IF_FALSE_GOTO, elseLabel, cond, "", ifStmt->line);
        generateStmt(ifStmt->thenBranch.get(), instructions, tempGen, loopStack);

        if (ifStmt->elseBranch) {
            instructions.emplace_back(IROp::GOTO, endLabel, "", "", ifStmt->line);
            instructions.emplace_back(IROp::LABEL, elseLabel, "", "", ifStmt->line);
            generateStmt(ifStmt->elseBranch.get(), instructions, tempGen, loopStack);
            instructions.emplace_back(IROp::LABEL, endLabel, "", "", ifStmt->line);
        } else {
            instructions.emplace_back(IROp::LABEL, elseLabel, "", "", ifStmt->line);
        }
        return;
    }

    // 5. While loop
    if (auto whileStmt = dynamic_cast<const WhileStmtAST*>(stmt)) {
        std::string startLabel = tempGen.newLabel();
        std::string endLabel = tempGen.newLabel();

        loopStack.emplace_back(startLabel, endLabel);

        instructions.emplace_back(IROp::LABEL, startLabel, "", "", whileStmt->line);
        std::string cond = IRGenExpr::generateExpr(whileStmt->condition.get(), instructions, tempGen);
        instructions.emplace_back(IROp::IF_FALSE_GOTO, endLabel, cond, "", whileStmt->line);

        generateStmt(whileStmt->body.get(), instructions, tempGen, loopStack);
        instructions.emplace_back(IROp::GOTO, startLabel, "", "", whileStmt->line);
        instructions.emplace_back(IROp::LABEL, endLabel, "", "", whileStmt->line);

        loopStack.pop_back();
        return;
    }

    // 6. For loop
    if (auto forStmt = dynamic_cast<const ForStmtAST*>(stmt)) {
        std::string startLabel = tempGen.newLabel();
        std::string continueLabel = tempGen.newLabel();
        std::string endLabel = tempGen.newLabel();

        if (forStmt->initializer) {
            generateStmt(forStmt->initializer.get(), instructions, tempGen, loopStack);
        }

        loopStack.emplace_back(continueLabel, endLabel);

        instructions.emplace_back(IROp::LABEL, startLabel, "", "", forStmt->line);
        if (forStmt->condition) {
            std::string cond = IRGenExpr::generateExpr(forStmt->condition.get(), instructions, tempGen);
            instructions.emplace_back(IROp::IF_FALSE_GOTO, endLabel, cond, "", forStmt->line);
        }

        generateStmt(forStmt->body.get(), instructions, tempGen, loopStack);

        instructions.emplace_back(IROp::LABEL, continueLabel, "", "", forStmt->line);
        if (forStmt->increment) {
            IRGenExpr::generateExpr(forStmt->increment.get(), instructions, tempGen);
        }
        instructions.emplace_back(IROp::GOTO, startLabel, "", "", forStmt->line);
        instructions.emplace_back(IROp::LABEL, endLabel, "", "", forStmt->line);

        loopStack.pop_back();
        return;
    }

    // 7. Break & Continue
    if (auto brk = dynamic_cast<const BreakStmtAST*>(stmt)) {
        if (!loopStack.empty()) {
            instructions.emplace_back(IROp::GOTO, loopStack.back().breakLabel, "", "", brk->line);
        }
        return;
    }

    if (auto cnt = dynamic_cast<const ContinueStmtAST*>(stmt)) {
        if (!loopStack.empty()) {
            instructions.emplace_back(IROp::GOTO, loopStack.back().continueLabel, "", "", cnt->line);
        }
        return;
    }

    // 8. Return statement
    if (auto ret = dynamic_cast<const ReturnStmtAST*>(stmt)) {
        std::string val = "";
        if (ret->value) {
            val = IRGenExpr::generateExpr(ret->value.get(), instructions, tempGen);
        }
        instructions.emplace_back(IROp::RETURN, "", val, "", ret->line);
        return;
    }

    // 9. Print statement
    if (auto prt = dynamic_cast<const PrintStmtAST*>(stmt)) {
        std::string val = IRGenExpr::generateExpr(prt->expression.get(), instructions, tempGen);
        instructions.emplace_back(IROp::PRINT, "", val, "", prt->line);
        return;
    }

    // 10. Read statement
    if (auto rd = dynamic_cast<const ReadStmtAST*>(stmt)) {
        instructions.emplace_back(IROp::READ, "", rd->variableName, "", rd->line);
        return;
    }
}
