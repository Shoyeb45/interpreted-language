#pragma once

#include "../scanner/token.hpp"
#include <iostream>
#include <string>

enum NodeType { LITERAL, BINARY, GROUP, UNARY };

struct ASTNode {
    NodeType type;
    virtual ~ASTNode() = default;

    virtual std::string visualize() = 0;
};

// Number, false, true, null, string literals
struct Literal : ASTNode {
    Token token;

    Literal(Token token) {
        this->token = token;
        this->type = NodeType::LITERAL;
    }

    std::string visualize() override {
        if (token.type == TokenType::NUMBER || token.type == TokenType::STRING) {
            return " " + token.get_literal();
        } else {
            return " " + token.lexeme;
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
    
    std::string visualize() override {
        std::string visulz =  " (" + this->operation.lexeme;

        visulz += this->left->visualize();
        visulz += this->right->visualize();
        return visulz + ")";
    }
};

struct Group : ASTNode {
    ASTNode *ast_node;

    Group(ASTNode *ast_node) {
        this->type = NodeType::GROUP;
        this->ast_node = ast_node;
    }

    std::string visualize() override {
        std::string visulz = " (group";
        visulz += this->ast_node->visualize();
        return visulz + ")";
    }
};

struct Unary : ASTNode {
    Token token;
    ASTNode* child;

    Unary(Token op, ASTNode *child) {
        this->type = NodeType::UNARY;
        this->child = child;
        this->token = op;
    }

    std::string visualize() override {
        std::string visulz = " (" + token.lexeme;
        visulz += this->child->visualize();
        return visulz + ")";
    }
};
