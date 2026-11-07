#ifndef CONTROL_FLOW_PARSER_H
#define CONTROL_FLOW_PARSER_H

#include "AST.h"
#include "ParserCore.h"
#include <memory>
#include <functional>

// PS/CSC/23/0297 - Isaac Seyram Benson: Control Flow Statement Parsing (If, While, For, Break, Continue)

class ControlFlowParser {
public:
    using StmtParserFn = std::function<std::unique_ptr<StmtAST>()>;

    static std::unique_ptr<StmtAST> parseIf(ParserCore& core, const StmtParserFn& parseStmt);
    static std::unique_ptr<StmtAST> parseWhile(ParserCore& core, const StmtParserFn& parseStmt);
    static std::unique_ptr<StmtAST> parseFor(ParserCore& core, const StmtParserFn& parseStmt);
    static std::unique_ptr<StmtAST> parseBreak(ParserCore& core);
    static std::unique_ptr<StmtAST> parseContinue(ParserCore& core);
};

#endif // CONTROL_FLOW_PARSER_H
