#pragma once

#include "../scanner/token.hpp"
#include "node_types.hpp"
#include <iostream>
#include <string>

struct Expr {
    NodeType type;
    virtual ~Expr() = default;

    virtual std::string visualize() = 0;
};

// Number, false, true, null, string literals
struct Literal : Expr {
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

struct Binary : Expr {
    Expr *left;
    Expr *right;
    Token operation;

    Binary(Expr *left, Expr *right, Token op) {
        this->left = left;
        this->right = right;
        this->operation = op;
        this->type = NodeType::BINARY;
    }

    std::string visualize() override {
        std::string visulz = " (" + this->operation.lexeme;

        visulz += left ? left->visualize() : "";
        visulz += right ? right->visualize() : "";
        return visulz + ")";
    }
};

struct Group : Expr {
    Expr *ast_node;

    Group(Expr *ast_node) {
        this->type = NodeType::GROUP;
        this->ast_node = ast_node;
    }

    std::string visualize() override {
        std::string visulz = " (group";
        visulz += ast_node ? ast_node->visualize() : "";
        return visulz + ")";
    }
};

struct Unary : Expr {
    Token token;
    Expr *child;

    Unary(Token op, Expr *child) {
        this->type = NodeType::UNARY;
        this->child = child;
        this->token = op;
    }

    std::string visualize() override {
        std::string visulz = " (" + token.lexeme;
        visulz += child ? child->visualize() : "";
        return visulz + ")";
    }
};

struct Variable : Expr {
    Token identifier;
    Variable(Token identifier) {
        this->identifier = identifier;
        type = NodeType::VARIABLE;
    }

    std::string visualize() override {
        std::string visz = " variable: " + identifier.lexeme;
        return visz;
    }
};

struct Assign : Expr {
    Token identifier;
    Expr *value;
    
    Assign(Token identifier, Expr *value) {
        this->identifier = identifier;
        this->value = value;
        type = NodeType::ASSIGN;
    }

    std::string visualize() override {
        return "Assign " + identifier.lexeme + " " + (value ? value->visualize() : "");
    }
};