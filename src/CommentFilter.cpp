#include "CommentFilter.h"
#include <cctype>

// PS/CSC/23/0293 - Mohammed Abdul Hamid: CommentFilter implementation

void CommentFilter::skipWhitespaceAndComments(SourceBuffer& buffer, ErrorHandler& errorHandler) {
    while (!buffer.isAtEnd()) {
        char c = buffer.peek();

        // 1. Skip standard whitespace
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            buffer.advance();
            continue;
        }

        // 2. Check for comments
        if (c == '/') {
            char next = buffer.peekNext();
            
            // Single-line comment: // ...
            if (next == '/') {
                buffer.advance(); // consume first '/'
                buffer.advance(); // consume second '/'
                while (!buffer.isAtEnd() && buffer.peek() != '\n') {
                    buffer.advance();
                }
                continue;
            }

            // Multi-line comment: /* ... */
            if (next == '*') {
                int startLine = buffer.getLine();
                int startCol = buffer.getColumn();

                buffer.advance(); // consume '/'
                buffer.advance(); // consume '*'

                bool closed = false;
                while (!buffer.isAtEnd()) {
                    if (buffer.peek() == '*' && buffer.peekNext() == '/') {
                        buffer.advance(); // consume '*'
                        buffer.advance(); // consume '/'
                        closed = true;
                        break;
                    }
                    buffer.advance();
                }

                if (!closed) {
                    errorHandler.report(startLine, startCol, "Unterminated multi-line comment starting here.");
                }
                continue;
            }
        }

        // Neither whitespace nor comment; stop skipping
        break;
    }
}
