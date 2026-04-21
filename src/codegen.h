#pragma once
#include "ast.h"
#include <vector>
#include <string>
#include <memory>
#include<variant>

// ─────────────────────────────────────────────
//  BYTECODE INSTRUCTION SET
//  (Stack-based, like JVM / Python bytecode)
// ─────────────────────────────────────────────
enum class OpCode {
    PUSH,       // push constant onto stack
    LOAD,       // push variable value onto stack
    STORE,      // pop stack → store in variable
    ADD,
    SUB,
    MUL,
    DIV,
    CMP_LT,     // pop a,b; push (b < a)
    CMP_GT,
    CMP_EQ,
    JMP,        // unconditional jump to index
    JMP_FALSE,  // pop; jump if 0
    PRINT,      // pop and print top of stack
    HALT
};

struct Instruction {
    OpCode op;
    std::variant<double, std::string, int> operand; 

    // Constructors for convenience
    static Instruction push(double v)           { return { OpCode::PUSH, v }; }
    static Instruction load(const std::string& n){ return { OpCode::LOAD, n}; }
    static Instruction store(const std::string& n){ return { OpCode::STORE, n}; }
    static Instruction jmp(int t)               { return { OpCode::JMP, t }; }
    static Instruction jmpFalse(int t)          { return { OpCode::JMP_FALSE, t }; }
    static Instruction simple(OpCode op)        { return { op, 0}; }
};

// ─────────────────────────────────────────────
//  CODE GENERATOR
//  Walks the AST and emits Instructions
// ─────────────────────────────────────────────
class CodeGen {
public:
    std::vector<Instruction> generate(ProgramNode& prog);
    void printBytecode(const std::vector<Instruction>& code) const;

private:
    std::vector<Instruction> code;

    void emitStmt(ASTNode* node);
    void emitExpr(ASTNode* node);

    // Returns index of emitted instruction (for backpatching)
    int emit(Instruction instr);
    void patch(int idx, int target);  // fix jump target
};