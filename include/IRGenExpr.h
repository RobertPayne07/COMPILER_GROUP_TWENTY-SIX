#ifndef IR_GEN_EXPR_H
#define IR_GEN_EXPR_H

#include "AST.h"
#include "IRInstruction.h"
#include <string>
#include <vector>

// PS/CSC/23/0297 - Isaac Seyram Benson: Expression and Assignment IR Lowering

class IRGenExpr {
public:
    static std::string generateExpr(const ExprAST* expr,
                                    std::vector<IRInstruction>& instructions,
                                    TempGenerator& tempGen);
};

#endif // IR_GEN_EXPR_H
