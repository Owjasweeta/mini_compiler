#include "parser.h"
#include <stdexcept>

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), pos(0) {}

Token& Parser::peek() { return tokens[pos]; }

Token& Parser::advance() { return tokens[pos++]; }

bool Parser::check(TokenType t) { return peek().type == t; }

bool Parser::match(TokenType t) {
    if (check(t)) { advance(); return true; }
    return false;
}

void Parser::expect(TokenType t, const std::string& msg) {
    if (!match(t)) throw std::runtime_error("Parse error: " + msg + " (got '" + peek().value + "')");
}

// ── Top Level ──────────────────────────────────────────────────────────────

std::unique_ptr<ProgramNode> Parser::parse() { return parseProgram(); }

std::unique_ptr<ProgramNode> Parser::parseProgram() {
    auto prog = std::make_unique<ProgramNode>();
    while (!check(TokenType::END))
        prog->stmts.push_back(parseStmt());
    return prog;
}

// ── Statements ─────────────────────────────────────────────────────────────

NodePtr Parser::parseStmt() {
    if (check(TokenType::PRINT)) return parsePrint();
    if (check(TokenType::IF))    return parseIf();
    if (check(TokenType::WHILE)) return parseWhile();
    // Default: assignment
    return parseAssign();
}

NodePtr Parser::parseAssign() {
    if (!check(TokenType::IDENT))
        throw std::runtime_error("Expected variable name, got '" + peek().value + "'");
    std::string name = advance().value;
    expect(TokenType::ASSIGN, "Expected '=' after variable name");
    auto expr = parseExpr();
    expect(TokenType::SEMICOLON, "Expected ';' after assignment");
    return std::make_unique<AssignNode>(name, std::move(expr));
}

NodePtr Parser::parsePrint() {
    advance(); // consume 'print'
    auto expr = parseExpr();
    expect(TokenType::SEMICOLON, "Expected ';' after print");
    return std::make_unique<PrintNode>(std::move(expr));
}

NodePtr Parser::parseIf() {
    advance(); // consume 'if'
    expect(TokenType::LPAREN, "Expected '(' after if");
    auto cond = parseExpr();
    expect(TokenType::RPAREN, "Expected ')' after condition");

    auto node = std::make_unique<IfNode>();
    node->cond = std::move(cond);
    node->body = parseBlock();

    if (match(TokenType::ELSE))
        node->elseBody = parseBlock();

    return node;
}

NodePtr Parser::parseWhile() {
    advance(); // consume 'while'
    expect(TokenType::LPAREN, "Expected '(' after while");
    auto cond = parseExpr();
    expect(TokenType::RPAREN, "Expected ')' after condition");

    auto node = std::make_unique<WhileNode>();
    node->cond = std::move(cond);
    node->body = parseBlock();
    return node;
}

std::vector<NodePtr> Parser::parseBlock() {
    expect(TokenType::LBRACE, "Expected '{'");
    std::vector<NodePtr> stmts;
    while (!check(TokenType::RBRACE) && !check(TokenType::END))
        stmts.push_back(parseStmt());
    expect(TokenType::RBRACE, "Expected '}'");
    return stmts;
}

// ── Expressions (operator precedence) ─────────────────────────────────────

NodePtr Parser::parseExpr()    { return parseCompare(); }

NodePtr Parser::parseCompare() {
    auto left = parseAdd();
    while (check(TokenType::LT) || check(TokenType::GT) || check(TokenType::EQ)) {
        char op = (peek().type == TokenType::LT) ? '<'
                : (peek().type == TokenType::GT) ? '>'
                :                                  '=';
        advance();
        left = std::make_unique<BinOpNode>(op, std::move(left), parseAdd());
    }
    return left;
}

NodePtr Parser::parseAdd() {
    auto left = parseMul();
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        char op = (peek().type == TokenType::PLUS) ? '+' : '-';
        advance();
        left = std::make_unique<BinOpNode>(op, std::move(left), parseMul());
    }
    return left;
}

NodePtr Parser::parseMul() {
    auto left = parseUnary();
    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        char op = (peek().type == TokenType::STAR) ? '*' : '/';
        advance();
        left = std::make_unique<BinOpNode>(op, std::move(left), parseUnary());
    }
    return left;
}

NodePtr Parser::parseUnary() {
    if (match(TokenType::MINUS)) {
        // Represent -x as (0 - x)
        auto zero = std::make_unique<NumberNode>(0);
        return std::make_unique<BinOpNode>('-', std::move(zero), parseUnary());
    }
    return parsePrimary();
}

NodePtr Parser::parsePrimary() {
    if (check(TokenType::NUMBER)) {
        double val = std::stod(advance().value);
        return std::make_unique<NumberNode>(val);
    }
    if (check(TokenType::IDENT)) {
        return std::make_unique<IdentNode>(advance().value);
    }
    if (match(TokenType::LPAREN)) {
        auto expr = parseExpr();
        expect(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    throw std::runtime_error("Unexpected token: '" + peek().value + "'");
}