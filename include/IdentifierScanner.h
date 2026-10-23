#ifndef IDENTIFIER_SCANNER_H
#define IDENTIFIER_SCANNER_H

#include <string>
#include <unordered_set>

class IdentifierScanner{
    public:
    static bool isIdentifierStart(char ch);
    static bool isIdentifierPart(char ch);
    static bool isKeyword(const std::string& word);  

    private:
     static const std::unordered_set<std::string> keywords;
};


#endif

