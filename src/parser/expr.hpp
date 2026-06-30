#pragma once

#include "../scanner/token.hpp"
#include "node_types.hpp"
#include <iostream>
#include <string>
#include <vector>

struct Expr {
    NodeType type;
    virtual ~Expr() = default;

    virtual std::string visualize() = 0;

    friend std::ostream &operator<<(std::ostream &os, Expr *expr) {
        os << "Expr Node Type -> " << node_type_to_string(expr->type) << "\n";
        return os;
    }
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

struct Logical : Expr {
    Token op;
    Expr *right;
    Expr *left;

    Logical(Expr *left, Expr *right, Token op) {
        this->op = op;
        this->left = left;
        this->right = right;
        type = NodeType::LOGICAL;
    };

    std::string visualize() override {
        std::string visulz = " (" + this->op.lexeme;

        visulz += left ? left->visualize() : "";
        visulz += right ? right->visualize() : "";
        return visulz + ")";
    }
};

struct Call : Expr {
  public:
    Expr *calle;
    std::vector<Expr *> args;
    Token name;
    Call(Token name, Expr *calle, std::vector<Expr *> &args) {
        this->name = name;
        this->calle = calle;
        this->args = args;
        type = NodeType::CALL;
    }

    std::string visualize() override {
        std::string visz = " (fun call ";

        if (calle)
            visz += "calle: (" + calle->visualize() + ") ";
        visz += "(args: [";

        for (int i = 0; i < args.size(); i++) {
            if (args[i]) {
                visz += args[i]->visualize() + (i == args.size() - 1 ? "]" : ", ");
            }
        }

        return visz + ")";
    }
};

struct Get : Expr {
    Expr *expr;
    Token name;

    Get(Expr *expr, Token &name) {
        this->name = name;
        this->expr = expr;
        type = NodeType::GET;
    }

    std::string visualize() override {
        std::string visz = "(Get ";
        if (expr)
            visz += expr->visualize();
        return visz + ")";
    }
};

struct Set : Expr {
    Expr *expr, *value;
    Token name;

    Set(Expr *expr, Expr *value, Token &name) {
        this->expr = expr;
        this->value = value;
        this->name = name;
        type = NodeType::SET;
    }

    std::string visualize() override {
        std::string visz = "(Set ";
        if (expr)
            visz += expr->visualize() + ")";
        if (value)
            visz += value->visualize() + ")";

        return visz + ")";
    }
};