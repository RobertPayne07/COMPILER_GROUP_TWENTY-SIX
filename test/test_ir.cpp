#include "Parser.h"
#include "SymbolTable.h"
#include "SemanticErrorHandler.h"
#include "TypeCheckerExpr.h"
#include "SemanticValidatorStmt.h"
#include "IRProgram.h"
#include <iostream>
#include <cassert>
#include <sstream>

// PS/CSC/23/0303 - Jonathan Armah Mensah: Unit & Regression Test Suite for Phase 3 (Semantics & IR)

void testSymbolTableScoping() {
    SymbolTable table;
    Symbol s1("x", "int", SymbolKind::VARIABLE, 1, 1);
    assert(table.define(s1));

    // Duplicate in same scope fails
    assert(!table.define(s1));

    // Enter inner scope
    table.enterScope();
    Symbol s2("x", "float", SymbolKind::VARIABLE, 2, 1); // Shadowing allowed in inner scope
    assert(table.define(s2));
    assert(table.lookup("x")->type == "float");

    table.exitScope();
    assert(table.lookup("x")->type == "int");

    std::cout << "[PASS] testSymbolTableScoping\n";
}

void testSemanticErrorDetection() {
    std::string badCode = "void main() { int x = 5; x = \"str\"; y = 10; break; }";
    Parser parser = Parser::fromSource(badCode);
    auto program = parser.parseProgram();
    assert(!parser.hasErrors());

    SemanticAnalyzer analyzer;
    bool valid = analyzer.analyze(program.get());

    assert(!valid);
    assert(analyzer.hasErrors());
    assert(analyzer.getErrorHandler().getErrors().size() >= 3);

    std::cout << "[PASS] testSemanticErrorDetection\n";
}

void testExpressionIRGeneration() {
    std::string code = "void test() { int a = 2; int b = 3; int c = a + b * 4; }";
    Parser parser = Parser::fromSource(code);
    auto program = parser.parseProgram();

    SemanticAnalyzer analyzer;
    assert(analyzer.analyze(program.get()));

    IRProgram ir = IRGenerator::generate(program.get());
    assert(!ir.getInstructions().empty());

    std::ostringstream ss;
    ir.dump(ss);
    std::string irStr = ss.str();

    // Must find temp assignments and operations
    assert(irStr.find("func test:") != std::string::npos);
    assert(irStr.find("a = 2") != std::string::npos);
    assert(irStr.find("b = 3") != std::string::npos);
    assert(irStr.find("t0 = b * 4") != std::string::npos || irStr.find("*") != std::string::npos);
    assert(irStr.find("end_func test") != std::string::npos);

    std::cout << "[PASS] testExpressionIRGeneration\n";
}

void testControlFlowIRGeneration() {
    std::string code = "void testLoop() { int i = 0; while (i < 5) { i = i + 1; } }";
    Parser parser = Parser::fromSource(code);
    auto program = parser.parseProgram();

    SemanticAnalyzer analyzer;
    assert(analyzer.analyze(program.get()));

    IRProgram ir = IRGenerator::generate(program.get());
    std::ostringstream ss;
    ir.dump(ss);
    std::string irStr = ss.str();

    assert(irStr.find("label") != std::string::npos || irStr.find("L0:") != std::string::npos);
    assert(irStr.find("ifFalse") != std::string::npos);
    assert(irStr.find("goto") != std::string::npos);

    std::cout << "[PASS] testControlFlowIRGeneration\n";
}

void testEndToEndSemanticAndIR() {
    std::string code = "int multiply(int a, int b) { return a * b; }\n"
                       "void main() {\n"
                       "  int x = 4;\n"
                       "  int y = 5;\n"
                       "  int res = multiply(x, y);\n"
                       "  print(res);\n"
                       "}";
    Parser parser = Parser::fromSource(code);
    auto program = parser.parseProgram();
    assert(!parser.hasErrors());

    SemanticAnalyzer analyzer;
    assert(analyzer.analyze(program.get()));

    IRProgram ir = IRGenerator::generate(program.get());
    assert(ir.getInstructions().size() > 5);

    std::cout << "[PASS] testEndToEndSemanticAndIR\n";
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "        RUNNING PHASE 3 SEMANTICS & IR TEST SUITE       \n";
    std::cout << "========================================================\n";
    testSymbolTableScoping();
    testSemanticErrorDetection();
    testExpressionIRGeneration();
    testControlFlowIRGeneration();
    testEndToEndSemanticAndIR();
    std::cout << "========================================================\n";
    std::cout << "All Phase 3 Semantics & IR tests passed successfully!\n";
    std::cout << "========================================================\n";
    return 0;
}
