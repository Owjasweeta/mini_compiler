#include "ast.h"
#include <iostream>
#include <string>

static std::string pad(int indent) { return std::string(indent * 2, ' '); }

void NumberNode::print(int indent) const {
    std::cout << pad(indent) << "Number(" << value << ")\n";
}

void IdentNode::print(int indent) const {
    std::cout << pad(indent) << "Ident(" << name << ")\n";
}

void BinOpNode::print(int indent) const {
    std::string opStr = (op == '=') ? "==" : std::string(1, op);
    std::cout << pad(indent) << "BinOp(" << opStr << ")\n";
    left->print(indent + 1);
    right->print(indent + 1);
}

void AssignNode::print(int indent) const {
    std::cout << pad(indent) << "Assign(" << name << ")\n";
    expr->print(indent + 1);
}

void PrintNode::print(int indent) const {
    std::cout << pad(indent) << "Print\n";
    expr->print(indent + 1);
}

void IfNode::print(int indent) const {
    std::cout << pad(indent) << "If\n";
    std::cout << pad(indent + 1) << "Cond:\n";
    cond->print(indent + 2);
    std::cout << pad(indent + 1) << "Then:\n";
    for (auto& s : body) s->print(indent + 2);
    if (!elseBody.empty()) {
        std::cout << pad(indent + 1) << "Else:\n";
        for (auto& s : elseBody) s->print(indent + 2);
    }
}

void WhileNode::print(int indent) const {
    std::cout << pad(indent) << "While\n";
    std::cout << pad(indent + 1) << "Cond:\n";
    cond->print(indent + 2);
    std::cout << pad(indent + 1) << "Body:\n";
    for (auto& s : body) s->print(indent + 2);
}

void ProgramNode::print(int indent) const {
    std::cout << pad(indent) << "Program\n";
    for (auto& s : stmts) s->print(indent + 1);
}
