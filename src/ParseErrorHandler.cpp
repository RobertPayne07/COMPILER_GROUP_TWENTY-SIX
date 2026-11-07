#include "ParseErrorHandler.h"

// PS/CSC/23/0293 - Mohammed Abdul Hamid: ParseErrorHandler implementation

void ParseErrorHandler::report(const Token& token, const std::string& message) {
    std::string lex = token.type == TokenType::TOKEN_EOF ? "end of file" : ("'" + token.lexeme + "'");
    errors.emplace_back(token.line, token.column, message, lex);
}

void ParseErrorHandler::report(int line, int column, const std::string& message) {
    errors.emplace_back(line, column, message, "");
}

bool ParseErrorHandler::hasErrors() const {
    return !errors.empty();
}

const std::vector<ParseError>& ParseErrorHandler::getErrors() const {
    return errors;
}

void ParseErrorHandler::printAll(std::ostream& out) const {
    for (const auto& err : errors) {
        out << "[Syntax Error] line " << err.line
            << ", col " << err.column
            << ": " << err.message;
        if (!err.tokenLexeme.empty()) {
            out << " (at " << err.tokenLexeme << ")";
        }
        out << "\n";
    }
}

void ParseErrorHandler::clear() {
    errors.clear();
}
