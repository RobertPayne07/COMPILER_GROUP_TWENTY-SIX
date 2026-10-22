#ifndef SOURCE_BUFFER_H
#define SOURCE_BUFFER_H

#include <string>

// PS/CSC/23/0292 - Yassin Alhassan: Source Code Stream & Coordinate Management

class SourceBuffer {
private:
    std::string source;
    size_t cursor;
    int line;
    int column;

public:
    explicit SourceBuffer(std::string sourceCode);
    static SourceBuffer fromFile(const std::string& filepath);

    char peek() const;
    char peekNext() const;
    char advance();
    bool match(char expected);
    bool isAtEnd() const;

    int getLine() const { return line; }
    int getColumn() const { return column; }
    size_t getCursor() const { return cursor; }
    const std::string& getSource() const { return source; }
};

#endif // SOURCE_BUFFER_H
