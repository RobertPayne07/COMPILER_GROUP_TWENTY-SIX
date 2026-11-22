#include "IRInstruction.h"
#include <sstream>

// PS/CSC/23/0295 - Judah Fiifi Tawiah: IRInstruction and TempGenerator Implementation

TempGenerator::TempGenerator() : tempCount(0), labelCount(0) {}

std::string TempGenerator::newTemp() {
    return "t" + std::to_string(tempCount++);
}

std::string TempGenerator::newLabel() {
    return "L" + std::to_string(labelCount++);
}

void TempGenerator::reset() {
    tempCount = 0;
    labelCount = 0;
}

std::string irOpToString(IROp op) {
    switch (op) {
        case IROp::ADD:          return "+";
        case IROp::SUB:          return "-";
        case IROp::MUL:          return "*";
        case IROp::DIV:          return "/";
        case IROp::MOD:          return "%";
        case IROp::NEG:          return "neg";
        case IROp::EQ:           return "==";
        case IROp::NEQ:          return "!=";
        case IROp::LT:           return "<";
        case IROp::LTE:          return "<=";
        case IROp::GT:           return ">";
        case IROp::GTE:          return ">=";
        case IROp::LOGICAL_AND:  return "&&";
        case IROp::LOGICAL_OR:   return "||";
        case IROp::LOGICAL_NOT:  return "!";
        case IROp::ASSIGN:       return "=";
        case IROp::LABEL:        return "label";
        case IROp::GOTO:         return "goto";
        case IROp::IF_FALSE_GOTO:return "ifFalse";
        case IROp::IF_TRUE_GOTO: return "ifTrue";
        case IROp::PARAM:        return "param";
        case IROp::CALL:         return "call";
        case IROp::RETURN:       return "return";
        case IROp::FUNC_BEGIN:   return "func_begin";
        case IROp::FUNC_END:     return "func_end";
        case IROp::PRINT:        return "print";
        case IROp::READ:         return "read";
        default:                 return "unknown";
    }
}

std::string IRInstruction::toString() const {
    std::ostringstream ss;
    switch (op) {
        case IROp::LABEL:
            ss << result << ":";
            break;
        case IROp::GOTO:
            ss << "  goto " << result;
            break;
        case IROp::IF_FALSE_GOTO:
            ss << "  ifFalse " << arg1 << " goto " << result;
            break;
        case IROp::IF_TRUE_GOTO:
            ss << "  ifTrue " << arg1 << " goto " << result;
            break;
        case IROp::ASSIGN:
            ss << "  " << result << " = " << arg1;
            break;
        case IROp::NEG:
            ss << "  " << result << " = -" << arg1;
            break;
        case IROp::LOGICAL_NOT:
            ss << "  " << result << " = !" << arg1;
            break;
        case IROp::ADD:
        case IROp::SUB:
        case IROp::MUL:
        case IROp::DIV:
        case IROp::MOD:
        case IROp::EQ:
        case IROp::NEQ:
        case IROp::LT:
        case IROp::LTE:
        case IROp::GT:
        case IROp::GTE:
        case IROp::LOGICAL_AND:
        case IROp::LOGICAL_OR:
            ss << "  " << result << " = " << arg1 << " " << irOpToString(op) << " " << arg2;
            break;
        case IROp::PARAM:
            ss << "  param " << arg1;
            break;
        case IROp::CALL:
            if (!result.empty()) {
                ss << "  " << result << " = call " << arg1 << ", " << arg2;
            } else {
                ss << "  call " << arg1 << ", " << arg2;
            }
            break;
        case IROp::RETURN:
            if (!arg1.empty()) {
                ss << "  return " << arg1;
            } else {
                ss << "  return";
            }
            break;
        case IROp::FUNC_BEGIN:
            ss << "func " << result << ":";
            break;
        case IROp::FUNC_END:
            ss << "end_func " << result << "\n";
            break;
        case IROp::PRINT:
            ss << "  print " << arg1;
            break;
        case IROp::READ:
            ss << "  read " << arg1;
            break;
    }
    return ss.str();
}
