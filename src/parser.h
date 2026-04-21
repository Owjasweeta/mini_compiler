#pragma once
#include "lexer.h"
#include "ast.h"
#include <vector>

// ─────────────────────────────────────────────
//  RECURSIVE DESCENT PARSER
//  Grammar:
//    program  := stmt*
//    stmt     := assign | print | if | while
//    assign   := IDENT '=' expr ';'
//    print    := 'print' expr ';'
//    if       := 'if' '(' expr ')' block ( 'else' block )?
//    while    := 'while' '(' expr ')' block
//    block    := '{' stmt* '}'
//    expr     := compare
//    compare  := add ( ('<'|'>'|'==') add )*
//    add      := mul  ( ('+'|'-') mul  )*
//    mul      := unary( ('*'|'/') unary)*
//    unary    := '-' unary | primary
//    primary  := NUMBER | IDENT | '(' expr ')'
// ─────────────────────────────────────────────
class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::unique_ptr<ProgramNode> parse();

private:
    std::vector<Token> tokens;
    size_t pos;

    Token& peek();
    Token& advance();
    bool check(TokenType t);
    bool match(TokenType t);
    void expect(TokenType t, const std::string& msg);

    std::unique_ptr<ProgramNode> parseProgram();
    NodePtr parseStmt();
    NodePtr parseAssign();
    NodePtr parsePrint();
    NodePtr parseIf();
    NodePtr parseWhile();
    std::vector<NodePtr> parseBlock();

    NodePtr parseExpr();
    NodePtr parseCompare();
    NodePtr parseAdd();
    NodePtr parseMul();
    NodePtr parseUnary();
    NodePtr parsePrimary();
};