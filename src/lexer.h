#pragma once
#include <string>
#include <vector>

// ─────────────────────────────────────────────
//  TOKEN TYPES
// ─────────────────────────────────────────────
enum class TokenType {
    NUMBER,     // 42, 3.14
    IDENT,      // variable names: x, result
    PLUS,       // +
    MINUS,      // -
    STAR,       // *
    SLASH,      // /
    ASSIGN,     // =
    LPAREN,     // (
    RPAREN,     // )
    IF,         // if
    ELSE,       // else
    WHILE,      // while
    LBRACE,     // {
    RBRACE,     // }
    LT,         // <
    GT,         // >
    EQ,         // ==
    PRINT,      // print
    SEMICOLON,  // ;
    END         // end of input
};

struct Token {
    TokenType type;
    std::string value;
};

// ─────────────────────────────────────────────
//  LEXER
// ─────────────────────────────────────────────
class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t pos;

    char peek();
    char advance();
    void skipWhitespaceAndComments();
    Token readNumber();
    Token readIdentOrKeyword();
};