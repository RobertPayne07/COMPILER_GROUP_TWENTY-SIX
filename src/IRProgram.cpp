#include "IRProgram.h"
#include "SemanticValidatorStmt.h"
#include "IRGenControlFlow.h"
#include "IRGenExpr.h"

// PS/CSC/23/0301: IRProgram and SemanticAnalyzer Implementation

void IRProgram::addInstruction(const IRInstruction& inst) {
    instructions.push_back(inst);
}

void IRProgram::dump(std::ostream& out) const {
    for (const auto& inst : instructions) {
        out << inst.toString() << "\n";
    }
}

bool SemanticAnalyzer::analyze(const ProgramAST* program) {
    if (!program) return false;
    SemanticValidatorStmt validator;
    validator.validateProgram(program, symbolTable, errorHandler);
    return !errorHandler.hasErrors();
}

IRProgram IRGenerator::generate(const ProgramAST* program) {
    IRProgram ir;
    if (!program) return ir;

    TempGenerator tempGen;
    std::vector<IRInstruction> instructions;
    std::vector<LoopContext> loopStack;

    // 1. Lower global variables
    for (const auto& decl : program->declarations) {
        if (auto var = dynamic_cast<const VarDeclAST*>(decl.get())) {
            IRGenControlFlow::generateStmt(var, instructions, tempGen, loopStack);
        }
    }

    // 2. Lower functions
    for (const auto& decl : program->declarations) {
        if (auto func = dynamic_cast<const FunctionDeclAST*>(decl.get())) {
            instructions.emplace_back(IROp::FUNC_BEGIN, func->functionName, "", "", func->line);

            if (func->body) {
                for (const auto& s : func->body->statements) {
                    IRGenControlFlow::generateStmt(s.get(), instructions, tempGen, loopStack);
                }
            }

            // Ensure void functions have a return instruction before func_end
            if (instructions.empty() || instructions.back().op != IROp::RETURN) {
                instructions.emplace_back(IROp::RETURN, "", "", "", func->line);
            }

            instructions.emplace_back(IROp::FUNC_END, func->functionName, "", "", func->line);
        }
    }

    for (auto& inst : instructions) {
        ir.addInstruction(inst);
    }

    return ir;
}
