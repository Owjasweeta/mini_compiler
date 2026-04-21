#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"

// Read entire file into string
static std::string readFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("Cannot open file: " + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static void usage() {
    std::cerr << "Usage:\n"
              << "  minicompiler <file.mc>            -- run program\n"
              << "  minicompiler <file.mc> --emit-ir  -- show bytecode then run\n"
              << "  minicompiler <file.mc> --emit-ast -- show AST then run\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) { usage(); return 1; }

    std::string path = argv[1];
    bool emitIR  = false;
    bool emitAST = false;

    for (int i = 2; i < argc; ++i) {
        std::string flag = argv[i];
        if (flag == "--emit-ir")  emitIR  = true;
        if (flag == "--emit-ast") emitAST = true;
    }

    try {
        // ── 1. Read Source ──────────────────────────────
        std::string source = readFile(path);

        // ── 2. Lex ──────────────────────────────────────
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        // ── 3. Parse → AST ──────────────────────────────
        Parser parser(std::move(tokens));
        auto ast = parser.parse();

        if (emitAST) {
            std::cout << "\n╔══════════════════════════════════════╗\n";
            std::cout <<   "║          ABSTRACT SYNTAX TREE        ║\n";
            std::cout <<   "╚══════════════════════════════════════╝\n";
            ast->print(1);
        }

        // ── 4. Code Generation → Bytecode ───────────────
        CodeGen codeGen;
        auto bytecode = codeGen.generate(*ast);

        if (emitIR) {
            codeGen.printBytecode(bytecode);
        }

        // ── 5. Execute ──────────────────────────────────
        std::cout << "\n── Output ──────────────────────────────\n";
        VM vm;
        vm.run(bytecode);
        std::cout << "────────────────────────────────────────\n";

    } catch (const std::exception& e) {
        std::cerr << "\n[ERROR] " << e.what() << "\n";
        return 1;
    }

    return 0;
}