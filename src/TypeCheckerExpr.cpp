#include "TypeCheckerExpr.h"

// PS/CSC/23/0293 - Mohammed Abdul Hamid: TypeCheckerExpr Implementation

std::string TypeCheckerExpr::checkExpression(const ExprAST* expr,
                                             const SymbolTable& symTab,
                                             SemanticErrorHandler& errHandler) {
    if (!expr) return "void";

    // 1. Literal expressions
    if (auto lit = dynamic_cast<const LiteralExprAST*>(expr)) {
        switch (lit->literalType) {
            case TokenType::LITERAL_INT:    return "int";
            case TokenType::LITERAL_FLOAT:  return "float";
            case TokenType::LITERAL_STRING: return "string";
            case TokenType::LITERAL_CHAR:   return "char";
            case TokenType::LITERAL_BOOL:   return "bool";
            default:                        return "unknown_type";
        }
    }

    // 2. Variable expressions
    if (auto var = dynamic_cast<const VariableExprAST*>(expr)) {
        const Symbol* sym = symTab.lookup(var->name);
        if (!sym) {
            errHandler.report(var->line, var->column, "Undeclared variable '" + var->name + "'.");
            return "error_type";
        }
        return sym->type;
    }

    // 3. Unary expressions
    if (auto un = dynamic_cast<const UnaryExprAST*>(expr)) {
        std::string opType = checkExpression(un->operand.get(), symTab, errHandler);
        if (opType == "error_type") return "error_type";

        if (un->op == TokenType::OP_LOGICAL_NOT) {
            if (!TypeUtils::isBoolean(opType)) {
                errHandler.report(un->line, un->column,
                    "Operator '!' requires boolean operand, found '" + opType + "'.");
                return "error_type";
            }
            return "bool";
        }

        if (un->op == TokenType::OP_MINUS || un->op == TokenType::OP_PLUS) {
            if (!TypeUtils::isNumeric(opType)) {
                errHandler.report(un->line, un->column,
                    "Unary arithmetic operator '" + un->opLexeme + "' requires numeric operand, found '" + opType + "'.");
                return "error_type";
            }
            return opType;
        }
        return opType;
    }

    // 4. Binary expressions
    if (auto bin = dynamic_cast<const BinaryExprAST*>(expr)) {
        std::string leftType = checkExpression(bin->left.get(), symTab, errHandler);
        std::string rightType = checkExpression(bin->right.get(), symTab, errHandler);

        if (leftType == "error_type" || rightType == "error_type") {
            return "error_type";
        }

        // Arithmetic: +, -, *, /, %
        if (bin->op == TokenType::OP_PLUS || bin->op == TokenType::OP_MINUS ||
            bin->op == TokenType::OP_MULTIPLY || bin->op == TokenType::OP_DIVIDE) {
            if (!TypeUtils::isNumeric(leftType) || !TypeUtils::isNumeric(rightType)) {
                errHandler.report(bin->line, bin->column,
                    "Arithmetic operator '" + bin->opLexeme + "' requires numeric operands, found '" +
                    leftType + "' and '" + rightType + "'.");
                return "error_type";
            }
            return TypeUtils::promoteBinaryType(leftType, rightType);
        }

        if (bin->op == TokenType::OP_MODULO) {
            if (leftType != "int" || rightType != "int") {
                errHandler.report(bin->line, bin->column,
                    "Modulo operator '%' requires integer operands, found '" +
                    leftType + "' and '" + rightType + "'.");
                return "error_type";
            }
            return "int";
        }

        // Relational: <, <=, >, >=
        if (bin->op == TokenType::OP_LESS || bin->op == TokenType::OP_LESS_EQ ||
            bin->op == TokenType::OP_GREATER || bin->op == TokenType::OP_GREATER_EQ) {
            if (!TypeUtils::isNumeric(leftType) || !TypeUtils::isNumeric(rightType)) {
                errHandler.report(bin->line, bin->column,
                    "Relational operator '" + bin->opLexeme + "' requires numeric operands, found '" +
                    leftType + "' and '" + rightType + "'.");
                return "error_type";
            }
            return "bool";
        }

        // Equality: ==, !=
        if (bin->op == TokenType::OP_EQUAL || bin->op == TokenType::OP_NOT_EQUAL) {
            if (!TypeUtils::areTypesCompatible(leftType, rightType) &&
                !TypeUtils::areTypesCompatible(rightType, leftType)) {
                errHandler.report(bin->line, bin->column,
                    "Equality operator '" + bin->opLexeme + "' cannot compare incompatible types '" +
                    leftType + "' and '" + rightType + "'.");
                return "error_type";
            }
            return "bool";
        }

        // Logical: &&, ||
        if (bin->op == TokenType::OP_LOGICAL_AND || bin->op == TokenType::OP_LOGICAL_OR) {
            if (!TypeUtils::isBoolean(leftType) || !TypeUtils::isBoolean(rightType)) {
                errHandler.report(bin->line, bin->column,
                    "Logical operator '" + bin->opLexeme + "' requires boolean operands, found '" +
                    leftType + "' and '" + rightType + "'.");
                return "error_type";
            }
            return "bool";
        }
    }

    // 5. Assignment expressions
    if (auto asgn = dynamic_cast<const AssignExprAST*>(expr)) {
        const Symbol* sym = symTab.lookup(asgn->targetName);
        if (!sym) {
            errHandler.report(asgn->line, asgn->column, "Undeclared assignment target '" + asgn->targetName + "'.");
            return "error_type";
        }

        std::string valType = checkExpression(asgn->value.get(), symTab, errHandler);
        if (valType == "error_type") return "error_type";

        if (!TypeUtils::areTypesCompatible(sym->type, valType)) {
            errHandler.report(asgn->line, asgn->column,
                "Cannot assign value of type '" + valType + "' to variable '" + asgn->targetName +
                "' of type '" + sym->type + "'.");
            return "error_type";
        }
        return sym->type;
    }

    // 6. Function calls
    if (auto call = dynamic_cast<const CallExprAST*>(expr)) {
        const Symbol* sym = symTab.lookup(call->callee);
        if (!sym) {
            errHandler.report(call->line, call->column, "Call to undeclared function '" + call->callee + "'.");
            return "error_type";
        }

        if (sym->kind != SymbolKind::FUNCTION) {
            errHandler.report(call->line, call->column, "'" + call->callee + "' is not a function.");
            return "error_type";
        }

        if (call->arguments.size() != sym->paramTypes.size()) {
            errHandler.report(call->line, call->column,
                "Function '" + call->callee + "' expects " + std::to_string(sym->paramTypes.size()) +
                " argument(s), but " + std::to_string(call->arguments.size()) + " provided.");
            return sym->type;
        }

        for (size_t i = 0; i < call->arguments.size(); ++i) {
            std::string argType = checkExpression(call->arguments[i].get(), symTab, errHandler);
            if (!TypeUtils::areTypesCompatible(sym->paramTypes[i], argType)) {
                errHandler.report(call->line, call->column,
                    "Argument " + std::to_string(i + 1) + " of function '" + call->callee +
                    "' expected type '" + sym->paramTypes[i] + "', but received '" + argType + "'.");
            }
        }

        return sym->type;
    }

    return "unknown_type";
}
