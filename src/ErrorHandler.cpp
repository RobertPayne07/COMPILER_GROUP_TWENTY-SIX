#include "ErrorHandler.h"

// PS/CSC/23/0299 - Kweku Andoh: ErrorHandler implementation

void ErrorHandler::report(int line, int column, const std::string& message) {
    errors.emplace_back(line, column, message);
}

bool ErrorHandler::hasErrors() const {
    return !errors.empty();
}

const std::vector<LexicalError>& ErrorHandler::getErrors() const {
    return errors;
}

void ErrorHandler::printAll(std::ostream& out) const {
    for (const auto& err : errors) {
        out << "[Lexical Error] line " << err.line 
            << ", col " << err.column 
            << ": " << err.message << "\n";
    }
}

void ErrorHandler::clear() {
    errors.clear();
}
