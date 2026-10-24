#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <vector>
#include <iostream>

// PS/CSC/23/0299 - Kweku Andoh: Error reporting and lexical diagnostics

struct LexicalError {
    int line;
    int column;
    std::string message;

    LexicalError(int line, int column, std::string message)
        : line(line), column(column), message(std::move(message)) {}
};

class ErrorHandler {
private:
    std::vector<LexicalError> errors;

public:
    void report(int line, int column, const std::string& message);
    bool hasErrors() const;
    const std::vector<LexicalError>& getErrors() const;
    void printAll(std::ostream& out = std::cerr) const;
    void clear();
};

#endif // ERROR_HANDLER_H
