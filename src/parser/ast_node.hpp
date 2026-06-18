#pragma once

#include "../scanner/token.hpp"
#include <iostream>
#include <string>

enum NodeType { LITERAL, BINARY, GROUP };

struct ASTNode {
    NodeType type;
    virtual ~ASTNode() = default;

    virtual void visualize() = 0;
};

// Number, false, true, null, string literals
struct Literal : ASTNode {
    Token token;

    Literal(Token token) {
        this->token = token;
        this->type = NodeType::LITERAL;
    }

    void visualize() override {
        if (token.type == TokenType::NUMBER) {
            std::cout << " " << token.get_literal();
        } else {
            std::cout << token.lexeme;
        }
    }
};

struct Binary : ASTNode {
    ASTNode *left;
    ASTNode *right;
    Token operation;

    Binary(ASTNode *left, ASTNode *right, Token op) {
        this->left = left;
        this->right = right;
        this->operation = op;
        this->type = NodeType::BINARY;
    }
    
    void visualize() override {
        std::cout << " (" << this->operation.lexeme;

        this->left->visualize();
        this->right->visualize();
        std::cout << ")";
    }
};

struct Group : ASTNode {
    ASTNode *ast_node;

    Group(ASTNode *ast_node) {
        this->type = NodeType::GROUP;
        this->ast_node = ast_node;
    }

    void visualize() override {
        std::cout << " (group";
        this->ast_node->visualize();
        std::cout << ")";
    }
};
