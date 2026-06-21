#pragma once

#include "expr.hpp"
#include "../core/utils.hpp"
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
    Expr* expr;
    PrintStmt(Expr* expr) {
        this->expr = expr;
        type = NodeType::PRINT_STMT;
    }

    std::string visualize() {
      std::string visz = "print (";
      if (expr) visz += trim(expr->visualize());
      return visz + ")";
    }
};

struct ExprStmt : Stmt { 
  public:
    Expr *expr;
    ExprStmt(Expr* expr) {
        this->expr = expr;
        type = NodeType::EXPR_STMT;
    }

    std::string visualize() {
      std::string visz = "";
      if (expr) visz += expr->visualize();
      return visz;
    }
};