#ifndef IR_INSTRUCTION_H
#define IR_INSTRUCTION_H

#include <string>
#include <vector>

// PS/CSC/23/0295 - Judah Fiifi Tawiah: Intermediate Representation (Three-Address Code) Architecture

enum class IROp {
    // Arithmetic
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    NEG,

    // Relational & Equality
    EQ,
    NEQ,
    LT,
    LTE,
    GT,
    GTE,

    // Logical
    LOGICAL_AND,
    LOGICAL_OR,
    LOGICAL_NOT,

    // Data movement
    ASSIGN,

    // Control Flow
    LABEL,
    GOTO,
    IF_FALSE_GOTO,
    IF_TRUE_GOTO,

    // Functions
    PARAM,
    CALL,
    RETURN,
    FUNC_BEGIN,
    FUNC_END,

    // I/O
    PRINT,
    READ
};

struct IRInstruction {
    IROp op;
    std::string result;
    std::string arg1;
    std::string arg2;
    int line;

    IRInstruction(IROp op, std::string result = "", std::string arg1 = "",
                  std::string arg2 = "", int line = 0)
        : op(op), result(std::move(result)), arg1(std::move(arg1)),
          arg2(std::move(arg2)), line(line) {}

    std::string toString() const;
};

class TempGenerator {
private:
    int tempCount;
    int labelCount;

public:
    TempGenerator();
    std::string newTemp();
    std::string newLabel();
    void reset();
};

std::string irOpToString(IROp op);

#endif // IR_INSTRUCTION_H
