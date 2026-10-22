#ifndef OPERATOR_SCANNER_H
#define OPERATOR_SCANNER_H

#include "Token.h"
#include "SourceBuffer.h"
#include "ErrorHandler.h"
#include <optional>

// PS/CSC/23/0297 - Isaac Seyram Benson: Operator and Delimiter Scanning

class OperatorScanner {
public:
    static std::optional<Token> scan(SourceBuffer& buffer, int startLine, int startCol, ErrorHandler& errorHandler);
};

#endif // OPERATOR_SCANNER_H
