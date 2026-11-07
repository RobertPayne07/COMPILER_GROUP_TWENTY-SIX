#include "AST.h"

// PS/CSC/23/0291 - Ernest Amoako Agyemang: AST Node string representation and helper utilities

std::string nodeTypeToString(NodeType type) {
    switch (type) {
        case NodeType::PROGRAM:        return "Program";
        case NodeType::VAR_DECL:       return "VarDecl";
        case NodeType::PARAM_DECL:     return "ParamDecl";
        case NodeType::FUNCTION_DECL:  return "FunctionDecl";
        case NodeType::BLOCK_STMT:     return "BlockStmt";
        case NodeType::EXPR_STMT:      return "ExprStmt";
        case NodeType::IF_STMT:        return "IfStmt";
        case NodeType::WHILE_STMT:     return "WhileStmt";
        case NodeType::FOR_STMT:       return "ForStmt";
        case NodeType::RETURN_STMT:    return "ReturnStmt";
        case NodeType::BREAK_STMT:     return "BreakStmt";
        case NodeType::CONTINUE_STMT:  return "ContinueStmt";
        case NodeType::PRINT_STMT:     return "PrintStmt";
        case NodeType::READ_STMT:      return "ReadStmt";
        case NodeType::BINARY_EXPR:    return "BinaryExpr";
        case NodeType::UNARY_EXPR:     return "UnaryExpr";
        case NodeType::LITERAL_EXPR:   return "LiteralExpr";
        case NodeType::VARIABLE_EXPR:  return "VariableExpr";
        case NodeType::ASSIGN_EXPR:    return "AssignExpr";
        case NodeType::CALL_EXPR:      return "CallExpr";
        default:                       return "UnknownNode";
    }
}
