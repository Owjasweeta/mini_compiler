#pragma once
#include <string>
#include <vector>
#include <memory>

// ─────────────────────────────────────────────
//  AST NODE BASE
// ─────────────────────────────────────────────
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

using NodePtr = std::unique_ptr<ASTNode>;

// ─────────────────────────────────────────────
//  EXPRESSION NODES
// ─────────────────────────────────────────────

// Numeric literal: 42, 3.14
struct NumberNode : ASTNode {
    double value;
    explicit NumberNode(double v) : value(v) {}
    void print(int indent = 0) const override;
};

// Variable reference: x
struct IdentNode : ASTNode {
    std::string name;
    explicit IdentNode(std::string n) : name(std::move(n)) {}
    void print(int indent = 0) const override;
};

// Binary operation: a + b, x * y, x < 5
struct BinOpNode : ASTNode {
    char op;  // '+', '-', '*', '/', '<', '>', '='(==)
    NodePtr left, right;
    BinOpNode(char op, NodePtr l, NodePtr r)
        : op(op), left(std::move(l)), right(std::move(r)) {}
    void print(int indent = 0) const override;
};

// ─────────────────────────────────────────────
//  STATEMENT NODES
// ─────────────────────────────────────────────

// Assignment: x = expr
struct AssignNode : ASTNode {
    std::string name;
    NodePtr expr;
    AssignNode(std::string n, NodePtr e) : name(std::move(n)), expr(std::move(e)) {}
    void print(int indent = 0) const override;
};

// print expr
struct PrintNode : ASTNode {
    NodePtr expr;
    explicit PrintNode(NodePtr e) : expr(std::move(e)) {}
    void print(int indent = 0) const override;
};

// if (cond) { body } else { elseBody }
struct IfNode : ASTNode {
    NodePtr cond;
    std::vector<NodePtr> body;
    std::vector<NodePtr> elseBody;
    void print(int indent = 0) const override;
};

// while (cond) { body }
struct WhileNode : ASTNode {
    NodePtr cond;
    std::vector<NodePtr> body;
    void print(int indent = 0) const override;
};

// A full program is a list of statements
struct ProgramNode : ASTNode {
    std::vector<NodePtr> stmts;
    void print(int indent = 0) const override;
};