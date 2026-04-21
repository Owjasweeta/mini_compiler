#include "vm.h"
#include <iostream>
#include <stdexcept>

void VM::push(double v) { stack.push(v); }

double VM::pop() {
    if (stack.empty()) throw std::runtime_error("VM stack underflow");
    double v = stack.top(); stack.pop(); return v;
}

void VM::run(const std::vector<Instruction>& code) {
    int ip = 0; // instruction pointer

    while (ip < (int)code.size()) {
        const Instruction& instr = code[ip];

        switch (instr.op) {
            case OpCode::PUSH:
                push(std::get<double>(instr.operand));
                break;

            case OpCode::LOAD: {
                const std::string& name = std::get<std::string>(instr.operand);
                auto it = variables.find(name);
                if (it == variables.end())
                    throw std::runtime_error("Undefined variable: " + name);
                push(it->second);
                break;
            }

            case OpCode::STORE: {
                const std::string& name = std::get<std::string>(instr.operand);
                variables[name] = pop();
                break;
            }

            case OpCode::ADD: { double b = pop(), a = pop(); push(a + b); break; }
            case OpCode::SUB: { double b = pop(), a = pop(); push(a - b); break; }
            case OpCode::MUL: { double b = pop(), a = pop(); push(a * b); break; }
            case OpCode::DIV: {
                double b = pop(), a = pop();
                if (b == 0) throw std::runtime_error("Division by zero");
                push(a / b);
                break;
            }

            case OpCode::CMP_LT: { double b = pop(), a = pop(); push(a < b ? 1.0 : 0.0); break; }
            case OpCode::CMP_GT: { double b = pop(), a = pop(); push(a > b ? 1.0 : 0.0); break; }
            case OpCode::CMP_EQ: { double b = pop(), a = pop(); push(a == b ? 1.0 : 0.0); break; }

            case OpCode::JMP:
                ip = std::get<int>(instr.operand);
                continue; // skip ip++

            case OpCode::JMP_FALSE: {
                double cond = pop();
                if (cond == 0.0) {
                    ip = std::get<int>(instr.operand);
                    continue;
                }
                break;
            }

            case OpCode::PRINT:
                std::cout << pop() << "\n";
                break;

            case OpCode::HALT:
                return;

            default:
                throw std::runtime_error("Unknown opcode");
        }

        ++ip;
    }
}