#pragma once
#include "expr.hpp"
#include <string>
#include <vector>

struct Parser {
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    Expr *root;
    int curr_index = 0;

    // helper methods

    /// Get current Token
    Token peek();
    /// Get the consumed token
    Token previous();
    /// move the parser
    Token advance();
    /// check if it's last token
    bool is_at_end();

    bool match(TokenType type);
    bool check(TokenType type);
    Expr *expression();
    Expr *term();
    Expr *factor();
    Expr *primary();
    Expr *unary();
    Expr *comparison();
    Expr *equality();

public:
    Parser(std::vector<Token> &tokens) {
        this->tokens = tokens;
    }

    Expr *parse() {
        return root = expression();
    }
    void visualize();
    void report_error();
    
    bool is_error() {
        return errors.size() > 0;
    }
};