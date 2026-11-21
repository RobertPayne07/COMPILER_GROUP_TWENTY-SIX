#include "SymbolTable.h"

// PS/CSC/23/0291 - Ernest Amoako Agyemang: SymbolTable Implementation

bool Scope::define(const Symbol& symbol) {
    if (symbols.find(symbol.name) != symbols.end()) {
        return false; // Already declared in this scope
    }
    symbols[symbol.name] = symbol;
    return true;
}

const Symbol* Scope::lookup(const std::string& name) const {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return &(it->second);
    }
    if (parent) {
        return parent->lookup(name);
    }
    return nullptr;
}

const Symbol* Scope::lookupCurrentScope(const std::string& name) const {
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        return &(it->second);
    }
    return nullptr;
}

SymbolTable::SymbolTable() {
    globalScope = std::make_shared<Scope>(nullptr, 0);
    currentScope = globalScope;
}

void SymbolTable::enterScope() {
    currentScope = std::make_shared<Scope>(currentScope, currentScope->getDepth() + 1);
}

void SymbolTable::exitScope() {
    if (currentScope->getParent()) {
        currentScope = currentScope->getParent();
    }
}

bool SymbolTable::define(const Symbol& symbol) {
    return currentScope->define(symbol);
}

const Symbol* SymbolTable::lookup(const std::string& name) const {
    return currentScope->lookup(name);
}

const Symbol* SymbolTable::lookupCurrentScope(const std::string& name) const {
    return currentScope->lookupCurrentScope(name);
}

int SymbolTable::getCurrentDepth() const {
    return currentScope->getDepth();
}
