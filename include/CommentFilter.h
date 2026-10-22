#ifndef COMMENT_FILTER_H
#define COMMENT_FILTER_H

#include "SourceBuffer.h"
#include "ErrorHandler.h"

// PS/CSC/23/0293 - Mohammed Abdul Hamid: Whitespace and comment filtering

class CommentFilter {
public:
    static void skipWhitespaceAndComments(SourceBuffer& buffer, ErrorHandler& errorHandler);
};

#endif // COMMENT_FILTER_H
