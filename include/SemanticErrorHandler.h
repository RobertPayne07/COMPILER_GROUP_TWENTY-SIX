#ifndef SEMANTIC_ERROR_HANDLER_H
#define SEMANTIC_ERROR_HANDLER_H

#include <string>
#include <vector>
#include <iostream>

// PS/CSC/23/0292 - Yassin Alhassan: Semantic Error Diagnostics and Type System Rules

struct SemanticError {
    int line;
    int column;
    std::string message;

    SemanticError(int line, int column, std::string message)
        : line(line), column(column), message(std::move(message)) {}
};

class TypeUtils {
public:
    static bool isNumeric(const std::string& type);
    static bool isBoolean(const std::string& type);
    static bool isVoid(const std::string& type);
    static bool areTypesCompatible(const std::string& expected, const std::string& actual);
    static std::string promoteBinaryType(const std::string& t1, const std::string& t2);
};

class SemanticErrorHandler {
private:
    std::vector<SemanticError> errors;

public:
    void report(int line, int column, const std::string& message);
    bool hasErrors() const;
    const std::vector<SemanticError>& getErrors() const;
    void printAll(std::ostream& out = std::cerr) const;
    void clear();
};

#endif // SEMANTIC_ERROR_HANDLER_H
