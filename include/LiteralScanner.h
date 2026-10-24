#ifndef LITERAL_SCANNER_H
#define LITERAL_SCANNER_H

#include "Token.h"
#include "SourceBuffer.h"
#include "ErrorHandler.h"

// PS/CSC/23/0295 - Judah Fiifi Tawiah: Numeric and String/Char Literal Scanning

class LiteralScanner {
public:
    static Token scanNumber(SourceBuffer& buffer, int startLine, int startCol);
    static Token scanString(SourceBuffer& buffer, int startLine, int startCol, ErrorHandler& errorHandler);
    static Token scanChar(SourceBuffer& buffer, int startLine, int startCol, ErrorHandler& errorHandler);
};

#endif // LITERAL_SCANNER_H
