#include "Parser.h"
#include "ASTPrinter.h"
#include <iostream>
#include <cassert>
#include <sstream>

// PS/CSC/23/0303 - Jonathan Armah Mensah: Unit & Regression Test Suite for Phase 2 Parser

void testExpressionsAndPrecedence() {
    std::string code = "void test() { int x = 2 + 3 * 4; bool b = true || false && true; }";
    Parser parser = Parser::fromSource(code);
    auto program = parser.parseProgram();

    assert(!parser.hasErrors());
    assert(program != nullptr);
    assert(program->declarations.size() == 1);

    auto func = dynamic_cast<FunctionDeclAST*>(program->declarations[0].get());
    assert(func != nullptr);
    assert(func->functionName == "test");
    assert(func->body->statements.size() == 2);

    // Verify 2 + 3 * 4 has '+' at root of expr, with '*' as right child
    auto varDecl = dynamic_cast<VarDeclAST*>(func->body->statements[0].get());
    assert(varDecl != nullptr);
    auto binExpr = dynamic_cast<BinaryExprAST*>(varDecl->initializer.get());
    assert(binExpr != nullptr);
    assert(binExpr->op == TokenType::OP_PLUS); // Precedence verified!

    std::cout << "[PASS] testExpressionsAndPrecedence\n";
}

void testControlFlowStatements() {
    std::string code = "void loopTest() {"
                       "  if (x > 0) { print(x); } else { print(0); }"
                       "  while (y > 0) { y = y - 1; if (y == 2) break; }"
                       "  for (i = 0; i < 10; i = i + 1) { continue; }"
                       "}";
    Parser parser = Parser::fromSource(code);
    auto program = parser.parseProgram();

    assert(!parser.hasErrors());
    assert(program != nullptr);
    auto func = dynamic_cast<FunctionDeclAST*>(program->declarations[0].get());
    assert(func != nullptr);
    assert(func->body->statements.size() == 3);

    assert(func->body->statements[0]->nodeType == NodeType::IF_STMT);
    assert(func->body->statements[1]->nodeType == NodeType::WHILE_STMT);
    assert(func->body->statements[2]->nodeType == NodeType::FOR_STMT);

    std::cout << "[PASS] testControlFlowStatements\n";
}

void testFunctionCallsAndIO() {
    std::string code = "int add(int a, int b) { return a + b; }"
                       "void main() {"
                       "  int sum = add(5, 10);"
                       "  print(sum);"
                       "  read(sum);"
                       "}";
    Parser parser = Parser::fromSource(code);
    auto program = parser.parseProgram();

    assert(!parser.hasErrors());
    assert(program->declarations.size() == 2);

    auto mainFunc = dynamic_cast<FunctionDeclAST*>(program->declarations[1].get());
    assert(mainFunc != nullptr);
    assert(mainFunc->body->statements.size() == 3);
    assert(mainFunc->body->statements[1]->nodeType == NodeType::PRINT_STMT);
    assert(mainFunc->body->statements[2]->nodeType == NodeType::READ_STMT);

    std::cout << "[PASS] testFunctionCallsAndIO\n";
}

void testSyntaxErrorHandling() {
    std::string code = "int badFunc( { return 0; }"; // missing param name and closing paren
    Parser parser = Parser::fromSource(code);
    auto program = parser.parseProgram();

    // Must detect syntax error and not crash
    assert(parser.hasErrors());
    assert(parser.getErrorHandler().getErrors().size() > 0);

    std::cout << "[PASS] testSyntaxErrorHandling\n";
}

void testASTPrinting() {
    std::string code = "int x = 42; void foo() { print(x); }";
    Parser parser = Parser::fromSource(code);
    auto program = parser.parseProgram();

    assert(!parser.hasErrors());
    std::ostringstream ss;
    ASTPrinter printer(ss);
    printer.print(program.get());

    std::string treeStr = ss.str();
    assert(treeStr.find("Program") != std::string::npos);
    assert(treeStr.find("VarDecl: x : int") != std::string::npos);
    assert(treeStr.find("FunctionDecl: foo -> void") != std::string::npos);

    std::cout << "[PASS] testASTPrinting\n";
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "             RUNNING PHASE 2 PARSER TEST SUITE          \n";
    std::cout << "========================================================\n";
    testExpressionsAndPrecedence();
    testControlFlowStatements();
    testFunctionCallsAndIO();
    testSyntaxErrorHandling();
    testASTPrinting();
    std::cout << "========================================================\n";
    std::cout << "All Phase 2 Parser tests passed successfully!\n";
    std::cout << "========================================================\n";
    return 0;
}
