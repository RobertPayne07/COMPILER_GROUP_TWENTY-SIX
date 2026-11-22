#include "IRGenExpr.h"

// PS/CSC/23/0297 - Isaac Seyram Benson: IRGenExpr Implementation

std::string IRGenExpr::generateExpr(const ExprAST* expr,
                                    std::vector<IRInstruction>& instructions,
                                    TempGenerator& tempGen) {
    if (!expr) return "";

    // 1. Literal expressions: return literal representation directly
    if (auto lit = dynamic_cast<const LiteralExprAST*>(expr)) {
        return lit->value;
    }

    // 2. Variable expressions: return variable identifier
    if (auto var = dynamic_cast<const VariableExprAST*>(expr)) {
        return var->name;
    }

    // 3. Unary expressions
    if (auto un = dynamic_cast<const UnaryExprAST*>(expr)) {
        std::string operand = generateExpr(un->operand.get(), instructions, tempGen);
        std::string temp = tempGen.newTemp();

        IROp op = (un->op == TokenType::OP_LOGICAL_NOT) ? IROp::LOGICAL_NOT : IROp::NEG;
        instructions.emplace_back(op, temp, operand, "", un->line);
        return temp;
    }

    // 4. Binary expressions
    if (auto bin = dynamic_cast<const BinaryExprAST*>(expr)) {
        std::string left = generateExpr(bin->left.get(), instructions, tempGen);
        std::string right = generateExpr(bin->right.get(), instructions, tempGen);
        std::string temp = tempGen.newTemp();

        IROp op = IROp::ADD;
        switch (bin->op) {
            case TokenType::OP_PLUS:        op = IROp::ADD; break;
            case TokenType::OP_MINUS:       op = IROp::SUB; break;
            case TokenType::OP_MULTIPLY:    op = IROp::MUL; break;
            case TokenType::OP_DIVIDE:      op = IROp::DIV; break;
            case TokenType::OP_MODULO:      op = IROp::MOD; break;
            case TokenType::OP_EQUAL:       op = IROp::EQ; break;
            case TokenType::OP_NOT_EQUAL:   op = IROp::NEQ; break;
            case TokenType::OP_LESS:        op = IROp::LT; break;
            case TokenType::OP_LESS_EQ:     op = IROp::LTE; break;
            case TokenType::OP_GREATER:     op = IROp::GT; break;
            case TokenType::OP_GREATER_EQ:  op = IROp::GTE; break;
            case TokenType::OP_LOGICAL_AND: op = IROp::LOGICAL_AND; break;
            case TokenType::OP_LOGICAL_OR:  op = IROp::LOGICAL_OR; break;
            default: break;
        }

        instructions.emplace_back(op, temp, left, right, bin->line);
        return temp;
    }

    // 5. Assignment expressions
    if (auto asgn = dynamic_cast<const AssignExprAST*>(expr)) {
        std::string val = generateExpr(asgn->value.get(), instructions, tempGen);
        instructions.emplace_back(IROp::ASSIGN, asgn->targetName, val, "", asgn->line);
        return asgn->targetName;
    }

    // 6. Function calls
    if (auto call = dynamic_cast<const CallExprAST*>(expr)) {
        std::vector<std::string> argTemps;
        for (const auto& arg : call->arguments) {
            argTemps.push_back(generateExpr(arg.get(), instructions, tempGen));
        }

        for (const auto& argTemp : argTemps) {
            instructions.emplace_back(IROp::PARAM, "", argTemp, "", call->line);
        }

        std::string temp = tempGen.newTemp();
        instructions.emplace_back(IROp::CALL, temp, call->callee,
                                  std::to_string(argTemps.size()), call->line);
        return temp;
    }

    return "";
}
