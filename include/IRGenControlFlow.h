#ifndef IR_GEN_CONTROL_FLOW_H
#define IR_GEN_CONTROL_FLOW_H

#include "AST.h"
#include "IRInstruction.h"
#include <string>
#include <vector>

// PS/CSC/23/0299 - Kweku Andoh: Statement and Control Flow IR Lowering

struct LoopContext {
    std::string continueLabel;
    std::string breakLabel;

    LoopContext(std::string continueLabel, std::string breakLabel)
        : continueLabel(std::move(continueLabel)), breakLabel(std::move(breakLabel)) {}
};

class IRGenControlFlow {
public:
    static void generateStmt(const StmtAST* stmt,
                             std::vector<IRInstruction>& instructions,
                             TempGenerator& tempGen,
                             std::vector<LoopContext>& loopStack);
};

#endif // IR_GEN_CONTROL_FLOW_H
