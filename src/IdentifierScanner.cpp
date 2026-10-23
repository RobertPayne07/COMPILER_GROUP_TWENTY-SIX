#include "IdentifierScanner.h"
#include <cctype>



const std::unordered_set<std::string> 
IdentifierScanner::keywords ={"if", "else", "int", "float", "double""char", "void", "return", "for", "while"};
    
bool IdentifierScanner::isIdentifierStart(char ch){
return(ch>='A' && ch<='Z') ||
     (ch>='a' && ch<='z') || ch=='_';    
}
bool IdentifierScanner::isIdentifierPart(char ch){
return (ch>='0' && ch<='9');
}


bool IdentifierScanner::isKeyword(const std::string& word){
return keywords.find(word)!=keywords.end();
}

