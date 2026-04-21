#include "lexer.h"
#include <stdexcept>
#include <cctype>

Lexer::Lexer(const std::string& source) : src(source), pos(0) {}

char Lexer::peek() {
    if (pos >= src.size()) return '\0';
    return src[pos];
}

char Lexer::advance() {
    return src[pos++];
}

void Lexer::skipWhitespaceAndComments() {
    while (pos < src.size()) {
        // Skip whitespace
        if (std::isspace(peek())) { advance(); continue; }
        // Skip // line comments
        if (peek() == '/' && pos + 1 < src.size() && src[pos+1] == '/') {
            while (pos < src.size() && peek() != '\n') advance();
            continue;
        }
        break;
    }
}

Token Lexer::readNumber() {
    std::string num;
    while (std::isdigit(peek()) || peek() == '.') num += advance();
    return { TokenType::NUMBER, num };
}

Token Lexer::readIdentOrKeyword() {
    std::string word;
    while (std::isalnum(peek()) || peek() == '_') word += advance();

    // Check keywords
    if (word == "if")    return { TokenType::IF,    word };
    if (word == "else")  return { TokenType::ELSE,  word };
    if (word == "while") return { TokenType::WHILE, word };
    if (word == "print") return { TokenType::PRINT, word };

    return { TokenType::IDENT, word };
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skipWhitespaceAndComments();
        if (pos >= src.size()) { tokens.push_back({ TokenType::END, "" }); break; }

        char c = peek();

        if (std::isdigit(c))            { tokens.push_back(readNumber()); continue; }
        if (std::isalpha(c) || c == '_'){ tokens.push_back(readIdentOrKeyword()); continue; }

        advance(); // consume the character
        switch (c) {
            case '+': tokens.push_back({ TokenType::PLUS,      "+" }); break;
            case '-': tokens.push_back({ TokenType::MINUS,     "-" }); break;
            case '*': tokens.push_back({ TokenType::STAR,      "*" }); break;
            case '/': tokens.push_back({ TokenType::SLASH,     "/" }); break;
            case '(': tokens.push_back({ TokenType::LPAREN,    "(" }); break;
            case ')': tokens.push_back({ TokenType::RPAREN,    ")" }); break;
            case '{': tokens.push_back({ TokenType::LBRACE,    "{" }); break;
            case '}': tokens.push_back({ TokenType::RBRACE,    "}" }); break;
            case ';': tokens.push_back({ TokenType::SEMICOLON, ";" }); break;
            case '<': tokens.push_back({ TokenType::LT,        "<" }); break;
            case '>': tokens.push_back({ TokenType::GT,        ">" }); break;
            case '=':
                if (peek() == '=') { advance(); tokens.push_back({ TokenType::EQ, "==" }); }
                else               { tokens.push_back({ TokenType::ASSIGN, "=" }); }
                break;
            default:
                throw std::runtime_error(std::string("Unknown character: ") + c);
        }
    }

    return tokens;
}