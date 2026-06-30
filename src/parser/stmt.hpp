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

    friend std::ostream &operator<<(std::ostream &os, Stmt *stmt) {
        os << "Stmt Node Type -> " << node_type_to_string(stmt->type) << "\n";
        return os;
    }
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
        std::string visz = "(var " + identifier.lexeme + " = ";
        if (expr)
            visz += expr->visualize();
        return visz + ")";
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
        for (auto &stmt : stmts)
            if (stmt)
                visz += stmt->visualize();
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
        std::string visz = "(if, condition: {";
        if (expr)
            visz += expr->visualize();
        visz += "} then -> {";
        
        if (then_branch) {
            visz += then_branch->visualize();
        }
        visz += "}";

        if (else_branch) {
            visz += " else branch -> {" + else_branch->visualize() + "}";
        }
        return visz + ")";
    }
};

struct WhileStmt : Stmt {
    Expr *condition;
    Stmt *body;

    WhileStmt(Expr *condition, Stmt *body) {
        type = NodeType::WHILE_STMT;
        this->condition = condition;
        this->body = body;
    }

    std::string visualize() override {
        std::string visz = "(while ";
        if (condition)
            visz += " condition -> (" + condition->visualize() + ")";
        if (body)
            visz += " body -> (" + body->visualize() + ")";
        return visz + ")";
    }
};

struct FuncStmt : Stmt {
  public:
    Token name;
    std::vector<Token> params;
    BlockStmt *body;

    FuncStmt(Token name, std::vector<Token> &params, BlockStmt *body) {
        this->name = name;
        this->params = params;
        this->body = body;
        type = NodeType::FUNCTION_STMT;
    }

    std::string visualize() override {
        std::string visz = " (fun decl, (params ";

        for (int i = 0; i < params.size(); i++) {
            visz += params[i].lexeme + (i == params.size() - 1 ? "" : ", ");
        }
        visz += ") (body: ";
        if (body)
            visz += "(" + body->visualize() + ")";
        return visz + ")";
    }
};

struct ReturnStmt : Stmt {
    Expr *expr;

    ReturnStmt(Expr *expr) {
        this->expr = expr;
        type = NodeType::RETURN_STMT;
    }

    std::string visualize() override {
        return " (return stmt: " + (expr ? expr->visualize() : "") + ")";
    }
};

struct ClassStmt : Stmt {
    Token name;
    std::vector<FuncStmt *> methods;

    ClassStmt(Token &name, std::vector<FuncStmt *> &methods) {
        type = NodeType::CLASS_STMT;
        this->name = name;
        this->methods = methods;
    }

    std::string visualize() override {
        std::string visz = "{class decl, name: " + name.lexeme + ", methods:\n";
        for (auto &method: methods) {
            visz += method->visualize() + "\n";
        }
        return visz + "}";
    }
};