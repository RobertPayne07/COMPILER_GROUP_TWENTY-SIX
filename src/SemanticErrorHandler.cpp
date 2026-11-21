#include "SemanticErrorHandler.h"

// PS/CSC/23/0292 - Yassin Alhassan: SemanticErrorHandler and TypeUtils implementation

bool TypeUtils::isNumeric(const std::string& type) {
    return type == "int" || type == "float";
}

bool TypeUtils::isBoolean(const std::string& type) {
    return type == "bool";
}

bool TypeUtils::isVoid(const std::string& type) {
    return type == "void";
}

bool TypeUtils::areTypesCompatible(const std::string& expected, const std::string& actual) {
    if (expected == actual) return true;
    // Automatic widening: int can be assigned to float
    if (expected == "float" && actual == "int") return true;
    return false;
}

std::string TypeUtils::promoteBinaryType(const std::string& t1, const std::string& t2) {
    if (t1 == "float" || t2 == "float") {
        return "float";
    }
    return "int";
}

void SemanticErrorHandler::report(int line, int column, const std::string& message) {
    errors.emplace_back(line, column, message);
}

bool SemanticErrorHandler::hasErrors() const {
    return !errors.empty();
}

const std::vector<SemanticError>& SemanticErrorHandler::getErrors() const {
    return errors;
}

void SemanticErrorHandler::printAll(std::ostream& out) const {
    for (const auto& err : errors) {
        out << "[Semantic Error] line " << err.line
            << ", col " << err.column
            << ": " << err.message << "\n";
    }
}

void SemanticErrorHandler::clear() {
    errors.clear();
}
