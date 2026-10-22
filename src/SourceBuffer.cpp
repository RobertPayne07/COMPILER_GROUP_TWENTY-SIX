#include "SourceBuffer.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// PS/CSC/23/0292 - Yassin Alhassan: SourceBuffer implementation

SourceBuffer::SourceBuffer(std::string sourceCode)
    : source(std::move(sourceCode)), cursor(0), line(1), column(1) {}

SourceBuffer SourceBuffer::fromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open source file: " + filepath);
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return SourceBuffer(ss.str());
}

char SourceBuffer::peek() const {
    if (isAtEnd()) return '\0';
    return source[cursor];
}

char SourceBuffer::peekNext() const {
    if (cursor + 1 >= source.length()) return '\0';
    return source[cursor + 1];
}

char SourceBuffer::advance() {
    if (isAtEnd()) return '\0';
    char current = source[cursor++];
    if (current == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return current;
}

bool SourceBuffer::match(char expected) {
    if (isAtEnd() || source[cursor] != expected) {
        return false;
    }
    advance();
    return true;
}

bool SourceBuffer::isAtEnd() const {
    return cursor >= source.length();
}
