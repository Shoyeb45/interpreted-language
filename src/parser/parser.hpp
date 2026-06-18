#pragma once
#include "ast_node.hpp"
#include <string>
#include <vector>

struct Parser {
    std::vector<Token> tokens;
    ASTNode *root;
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
    ASTNode *expression();
    ASTNode *term();
    ASTNode *factor();
    ASTNode *primary();

    void visualize(ASTNode *node);

  public:
    Parser(std::vector<Token> &tokens) {
        this->tokens = tokens;
    }

    ASTNode *parse() {
        return root = expression();
    }
    void visualize();
};