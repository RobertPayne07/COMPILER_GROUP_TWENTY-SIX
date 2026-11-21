#ifndef IR_PROGRAM_H
#define IR_PROGRAM_H

#include "AST.h"
#include "IRInstruction.h"
#include "SymbolTable.h"
#include "SemanticErrorHandler.h"
#include <vector>
#include <iostream>

// PS/CSC/23/0301: Semantic Analyzer Coordinator and IR Program Generator

class IRProgram {
private:
    std::vector<IRInstruction> instructions;

public:
    IRProgram() = default;
    void addInstruction(const IRInstruction& inst);
    const std::vector<IRInstruction>& getInstructions() const { return instructions; }
    void dump(std::ostream& out = std::cout) const;
};

class SemanticAnalyzer {
private:
    SymbolTable symbolTable;
    SemanticErrorHandler errorHandler;

public:
    SemanticAnalyzer() = default;
    bool analyze(const ProgramAST* program);
    const SymbolTable& getSymbolTable() const { return symbolTable; }
    const SemanticErrorHandler& getErrorHandler() const { return errorHandler; }
    bool hasErrors() const { return errorHandler.hasErrors(); }
};

class IRGenerator {
public:
    static IRProgram generate(const ProgramAST* program);
};

#endif // IR_PROGRAM_H
