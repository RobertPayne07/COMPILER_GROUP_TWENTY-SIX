#ifndef AST_H
#define AST_H

#include "Token.h"
#include <string>
#include <vector>
#include <memory>

// PS/CSC/23/0291 - Ernest Amoako Agyemang: Abstract Syntax Tree (AST) Base and Node Hierarchy

enum class NodeType {
    PROGRAM,
    VAR_DECL,
    PARAM_DECL,
    FUNCTION_DECL,
    BLOCK_STMT,
    EXPR_STMT,
    IF_STMT,
    WHILE_STMT,
    FOR_STMT,
    RETURN_STMT,
    BREAK_STMT,
    CONTINUE_STMT,
    PRINT_STMT,
    READ_STMT,
    BINARY_EXPR,
    UNARY_EXPR,
    LITERAL_EXPR,
    VARIABLE_EXPR,
    ASSIGN_EXPR,
    CALL_EXPR
};

// Base AST Node
class ASTNode {
public:
    NodeType nodeType;
    int line;
    int column;

    ASTNode(NodeType type, int line, int column)
        : nodeType(type), line(line), column(column) {}
    virtual ~ASTNode() = default;
};

// Expression Base Node
class ExprAST : public ASTNode {
public:
    ExprAST(NodeType type, int line, int column)
        : ASTNode(type, line, column) {}
};

// Statement Base Node
class StmtAST : public ASTNode {
public:
    StmtAST(NodeType type, int line, int column)
        : ASTNode(type, line, column) {}
};

// --- Concrete Expression Nodes ---

class LiteralExprAST : public ExprAST {
public:
    TokenType literalType;
    std::string value;

    LiteralExprAST(TokenType literalType, std::string value, int line, int column)
        : ExprAST(NodeType::LITERAL_EXPR, line, column),
          literalType(literalType), value(std::move(value)) {}
};

class VariableExprAST : public ExprAST {
public:
    std::string name;

    VariableExprAST(std::string name, int line, int column)
        : ExprAST(NodeType::VARIABLE_EXPR, line, column), name(std::move(name)) {}
};

class BinaryExprAST : public ExprAST {
public:
    TokenType op;
    std::string opLexeme;
    std::unique_ptr<ExprAST> left;
    std::unique_ptr<ExprAST> right;

    BinaryExprAST(TokenType op, std::string opLexeme,
                  std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right,
                  int line, int column)
        : ExprAST(NodeType::BINARY_EXPR, line, column),
          op(op), opLexeme(std::move(opLexeme)),
          left(std::move(left)), right(std::move(right)) {}
};

class UnaryExprAST : public ExprAST {
public:
    TokenType op;
    std::string opLexeme;
    std::unique_ptr<ExprAST> operand;

    UnaryExprAST(TokenType op, std::string opLexeme,
                 std::unique_ptr<ExprAST> operand, int line, int column)
        : ExprAST(NodeType::UNARY_EXPR, line, column),
          op(op), opLexeme(std::move(opLexeme)), operand(std::move(operand)) {}
};

class AssignExprAST : public ExprAST {
public:
    std::string targetName;
    std::unique_ptr<ExprAST> value;

    AssignExprAST(std::string targetName, std::unique_ptr<ExprAST> value, int line, int column)
        : ExprAST(NodeType::ASSIGN_EXPR, line, column),
          targetName(std::move(targetName)), value(std::move(value)) {}
};

class CallExprAST : public ExprAST {
public:
    std::string callee;
    std::vector<std::unique_ptr<ExprAST>> arguments;

    CallExprAST(std::string callee, std::vector<std::unique_ptr<ExprAST>> args, int line, int column)
        : ExprAST(NodeType::CALL_EXPR, line, column),
          callee(std::move(callee)), arguments(std::move(args)) {}
};

// --- Concrete Statement Nodes ---

class BlockStmtAST : public StmtAST {
public:
    std::vector<std::unique_ptr<StmtAST>> statements;

    BlockStmtAST(std::vector<std::unique_ptr<StmtAST>> stmts, int line, int column)
        : StmtAST(NodeType::BLOCK_STMT, line, column), statements(std::move(stmts)) {}
};

class VarDeclAST : public StmtAST {
public:
    std::string typeName;
    std::string varName;
    std::unique_ptr<ExprAST> initializer; // optional, can be nullptr

    VarDeclAST(std::string typeName, std::string varName,
               std::unique_ptr<ExprAST> init, int line, int column)
        : StmtAST(NodeType::VAR_DECL, line, column),
          typeName(std::move(typeName)), varName(std::move(varName)),
          initializer(std::move(init)) {}
};

class ParamDeclAST : public ASTNode {
public:
    std::string typeName;
    std::string paramName;

    ParamDeclAST(std::string typeName, std::string paramName, int line, int column)
        : ASTNode(NodeType::PARAM_DECL, line, column),
          typeName(std::move(typeName)), paramName(std::move(paramName)) {}
};

class FunctionDeclAST : public ASTNode {
public:
    std::string returnType;
    std::string functionName;
    std::vector<std::unique_ptr<ParamDeclAST>> parameters;
    std::unique_ptr<BlockStmtAST> body;

    FunctionDeclAST(std::string returnType, std::string functionName,
                    std::vector<std::unique_ptr<ParamDeclAST>> params,
                    std::unique_ptr<BlockStmtAST> body, int line, int column)
        : ASTNode(NodeType::FUNCTION_DECL, line, column),
          returnType(std::move(returnType)), functionName(std::move(functionName)),
          parameters(std::move(params)), body(std::move(body)) {}
};

class ExprStmtAST : public StmtAST {
public:
    std::unique_ptr<ExprAST> expression;

    ExprStmtAST(std::unique_ptr<ExprAST> expr, int line, int column)
        : StmtAST(NodeType::EXPR_STMT, line, column), expression(std::move(expr)) {}
};

class IfStmtAST : public StmtAST {
public:
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<StmtAST> thenBranch;
    std::unique_ptr<StmtAST> elseBranch; // optional, can be nullptr

    IfStmtAST(std::unique_ptr<ExprAST> cond, std::unique_ptr<StmtAST> thenBr,
             std::unique_ptr<StmtAST> elseBr, int line, int column)
        : StmtAST(NodeType::IF_STMT, line, column),
          condition(std::move(cond)), thenBranch(std::move(thenBr)), elseBranch(std::move(elseBr)) {}
};

class WhileStmtAST : public StmtAST {
public:
    std::unique_ptr<ExprAST> condition;
    std::unique_ptr<StmtAST> body;

    WhileStmtAST(std::unique_ptr<ExprAST> cond, std::unique_ptr<StmtAST> body, int line, int column)
        : StmtAST(NodeType::WHILE_STMT, line, column),
          condition(std::move(cond)), body(std::move(body)) {}
};

class ForStmtAST : public StmtAST {
public:
    std::unique_ptr<StmtAST> initializer; // VarDecl or ExprStmt or nullptr
    std::unique_ptr<ExprAST> condition;   // optional
    std::unique_ptr<ExprAST> increment;   // optional
    std::unique_ptr<StmtAST> body;

    ForStmtAST(std::unique_ptr<StmtAST> init, std::unique_ptr<ExprAST> cond,
               std::unique_ptr<ExprAST> inc, std::unique_ptr<StmtAST> body,
               int line, int column)
        : StmtAST(NodeType::FOR_STMT, line, column),
          initializer(std::move(init)), condition(std::move(cond)),
          increment(std::move(inc)), body(std::move(body)) {}
};

class ReturnStmtAST : public StmtAST {
public:
    std::unique_ptr<ExprAST> value; // optional

    ReturnStmtAST(std::unique_ptr<ExprAST> value, int line, int column)
        : StmtAST(NodeType::RETURN_STMT, line, column), value(std::move(value)) {}
};

class BreakStmtAST : public StmtAST {
public:
    BreakStmtAST(int line, int column)
        : StmtAST(NodeType::BREAK_STMT, line, column) {}
};

class ContinueStmtAST : public StmtAST {
public:
    ContinueStmtAST(int line, int column)
        : StmtAST(NodeType::CONTINUE_STMT, line, column) {}
};

class PrintStmtAST : public StmtAST {
public:
    std::unique_ptr<ExprAST> expression;

    PrintStmtAST(std::unique_ptr<ExprAST> expr, int line, int column)
        : StmtAST(NodeType::PRINT_STMT, line, column), expression(std::move(expr)) {}
};

class ReadStmtAST : public StmtAST {
public:
    std::string variableName;

    ReadStmtAST(std::string varName, int line, int column)
        : StmtAST(NodeType::READ_STMT, line, column), variableName(std::move(varName)) {}
};

// Root Program Node
class ProgramAST : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> declarations; // functions and global variables

    ProgramAST(std::vector<std::unique_ptr<ASTNode>> decls, int line, int column)
        : ASTNode(NodeType::PROGRAM, line, column), declarations(std::move(decls)) {}
};

std::string nodeTypeToString(NodeType type);

#endif // AST_H
