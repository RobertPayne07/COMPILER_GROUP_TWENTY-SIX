#include "Scanner.h"
#include <iostream>
#include <cassert>

// PS/CSC/23/0303 - Jonathan Armah Mensah: Unit & Regression Test Suite

void testKeywordsAndIdentifiers() {
    std::string code = "int count = 42; while (count > 0) { count = count - 1; }";
    Scanner scanner(code);
    auto tokens = scanner.scanTokens();

    assert(tokens.size() > 0);
    assert(tokens[0].type == TokenType::KEYWORD_INT);
    assert(tokens[1].type == TokenType::IDENTIFIER && tokens[1].lexeme == "count");
    assert(tokens[2].type == TokenType::OP_ASSIGN);
    assert(tokens[3].type == TokenType::LITERAL_INT && tokens[3].lexeme == "42");
    assert(tokens[4].type == TokenType::DELIM_SEMICOLON);
    assert(tokens[5].type == TokenType::KEYWORD_WHILE);
    std::cout << "[PASS] testKeywordsAndIdentifiers\n";
}

void testLiteralsAndEscapes() {
    std::string code = "string msg = \"hello \\n world\"; float pi = 3.1415; bool flag = true;";
    Scanner scanner(code);
    auto tokens = scanner.scanTokens();

    assert(tokens[0].type == TokenType::KEYWORD_STRING);
    assert(tokens[1].type == TokenType::IDENTIFIER);
    assert(tokens[2].type == TokenType::OP_ASSIGN);
    assert(tokens[3].type == TokenType::LITERAL_STRING);
    assert(tokens[5].type == TokenType::KEYWORD_FLOAT);
    assert(tokens[7].type == TokenType::OP_ASSIGN);
    assert(tokens[8].type == TokenType::LITERAL_FLOAT);
    assert(tokens[13].type == TokenType::LITERAL_BOOL);
    std::cout << "[PASS] testLiteralsAndEscapes\n";
}

void testCommentsAndWhitespace() {
    std::string code = "// this is a line comment\n"
                       "/* multi-line\n"
                       "   comment here */\n"
                       "int a = 10;";
    Scanner scanner(code);
    auto tokens = scanner.scanTokens();

    assert(tokens[0].type == TokenType::KEYWORD_INT);
    assert(tokens[0].line == 4); // Line coordinate correctly tracked!
    assert(tokens[1].lexeme == "a");
    assert(!scanner.hasErrors());
    std::cout << "[PASS] testCommentsAndWhitespace\n";
}

void testOperatorsAndDelimiters() {
    std::string code = "== != <= >= && || ! + - * / %";
    Scanner scanner(code);
    auto tokens = scanner.scanTokens();

    assert(tokens[0].type == TokenType::OP_EQUAL);
    assert(tokens[1].type == TokenType::OP_NOT_EQUAL);
    assert(tokens[2].type == TokenType::OP_LESS_EQ);
    assert(tokens[3].type == TokenType::OP_GREATER_EQ);
    assert(tokens[4].type == TokenType::OP_LOGICAL_AND);
    assert(tokens[5].type == TokenType::OP_LOGICAL_OR);
    assert(tokens[6].type == TokenType::OP_LOGICAL_NOT);
    assert(tokens[7].type == TokenType::OP_PLUS);
    assert(tokens[8].type == TokenType::OP_MINUS);
    assert(tokens[9].type == TokenType::OP_MULTIPLY);
    assert(tokens[10].type == TokenType::OP_DIVIDE);
    assert(tokens[11].type == TokenType::OP_MODULO);
    std::cout << "[PASS] testOperatorsAndDelimiters\n";
}

void testErrorHandling() {
    std::string code = "int valid = 5; @ # $ \"unterminated string";
    Scanner scanner(code);
    auto tokens = scanner.scanTokens();

    assert(scanner.hasErrors());
    assert(scanner.getErrorHandler().getErrors().size() >= 3);
    std::cout << "[PASS] testErrorHandling\n";
}

int main() {
    std::cout << "Running Scanner Test Suite...\n";
    testKeywordsAndIdentifiers();
    testLiteralsAndEscapes();
    testCommentsAndWhitespace();
    testOperatorsAndDelimiters();
    testErrorHandling();
    std::cout << "All Scanner tests passed successfully!\n";
    return 0;
}
