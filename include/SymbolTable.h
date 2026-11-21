#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>

// PS/CSC/23/0291 - Ernest Amoako Agyemang: Symbol Table and Scoping Architecture

enum class SymbolKind {
    VARIABLE,
    PARAMETER,
    FUNCTION
};

struct Symbol {
    std::string name;
    std::string type;
    SymbolKind kind;
    int line;
    int column;
    std::vector<std::string> paramTypes; // For function symbols

    Symbol() : kind(SymbolKind::VARIABLE), line(0), column(0) {}

    Symbol(std::string name, std::string type, SymbolKind kind, int line, int column,
           std::vector<std::string> paramTypes = {})
        : name(std::move(name)), type(std::move(type)), kind(kind),
          line(line), column(column), paramTypes(std::move(paramTypes)) {}
};

class Scope {
private:
    std::unordered_map<std::string, Symbol> symbols;
    std::shared_ptr<Scope> parent;
    int depth;

public:
    explicit Scope(std::shared_ptr<Scope> parent = nullptr, int depth = 0)
        : parent(std::move(parent)), depth(depth) {}

    bool define(const Symbol& symbol);
    const Symbol* lookup(const std::string& name) const;
    const Symbol* lookupCurrentScope(const std::string& name) const;

    std::shared_ptr<Scope> getParent() const { return parent; }
    int getDepth() const { return depth; }
    const std::unordered_map<std::string, Symbol>& getAllSymbols() const { return symbols; }
};

class SymbolTable {
private:
    std::shared_ptr<Scope> currentScope;
    std::shared_ptr<Scope> globalScope;

public:
    SymbolTable();

    void enterScope();
    void exitScope();

    bool define(const Symbol& symbol);
    const Symbol* lookup(const std::string& name) const;
    const Symbol* lookupCurrentScope(const std::string& name) const;

    int getCurrentDepth() const;
    std::shared_ptr<Scope> getGlobalScope() const { return globalScope; }
    std::shared_ptr<Scope> getCurrentScope() const { return currentScope; }
};

#endif // SYMBOL_TABLE_H
