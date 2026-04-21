#pragma once
#include "codegen.h"
#include <unordered_map>
#include <stack>
#include <string>

// ─────────────────────────────────────────────
//  VIRTUAL MACHINE
//  Stack-based executor for our bytecode
// ─────────────────────────────────────────────
class VM {
public:
    void run(const std::vector<Instruction>& code);

private:
    std::stack<double> stack;
    std::unordered_map<std::string, double> variables;

    void push(double v);
    double pop();
};