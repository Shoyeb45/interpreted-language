#pragma once

#include "../core/utils.hpp"
#include "expr.hpp"
#include "node_types.hpp"
#include <string>

struct Stmt {
  public:
    NodeType type;
    virtual ~Stmt() = default;
    virtual std::string visualize() = 0;
};

struct PrintStmt : Stmt {
  public:
    Expr *expr;
    PrintStmt(Expr *expr) {
        this->expr = expr;
        type = NodeType::PRINT_STMT;
    }

    std::string visualize() {
        std::string visz = "print (";
        if (expr)
            visz += trim(expr->visualize());
        return visz + ")";
    }
};

struct ExprStmt : Stmt {
  public:
    Expr *expr;
    ExprStmt(Expr *expr) {
        this->expr = expr;
        type = NodeType::EXPR_STMT;
    }

    std::string visualize() {
        std::string visz = "";
        if (expr)
            visz += expr->visualize();
        return visz;
    }
};

struct VariableStmt : Stmt {
  public:
    Expr *expr;
    Token identifier;
    VariableStmt(Expr *expr, Token identifier) {
        this->expr = expr;
        this->identifier = identifier;
        type = NodeType::VARIABLE_STMT;
    }

    std::string visualize() {
        std::string visz = "";
        if (expr)
            visz += expr->visualize();
        return visz;
    }
};

struct BlockStmt : Stmt {
  public:
    std::vector<Stmt *> stmts;
    BlockStmt(std::vector<Stmt *> &stmts) {
        this->stmts = stmts;
        type = NodeType::BLOCK_STMT;
    }

    std::string visualize() override {
        std::string visz = " {block ";
        for (auto &stmt: stmts) 
            if (stmt) visz += stmt->visualize();
        return visz + "}";
    }
};

struct IfStmt : Stmt {
  public:
    Expr *expr;
    Stmt *then_branch;
    Stmt *else_branch;

    IfStmt(Expr *expr, Stmt *then_branch, Stmt *else_branch) {
        type = NodeType::IF_STMT;
        this->then_branch = then_branch;
        this->expr = expr;
        this->else_branch = else_branch;
    }
    
    std::string visualize() override {
        std::string visz = "(if expression ";
        if (expr) visz += expr->visualize();
        return visz + ")";
    }
};