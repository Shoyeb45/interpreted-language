#pragma once

#include "../parser/stmt.hpp"
#include "../parser/expr.hpp"
#include "../evaluator/evaluator.hpp"
#include <vector>

struct Executor {
    std::vector<Stmt *> statements;
    Evaluator* evaluator = new Evaluator(nullptr);
    void execute_prnt_stmt(PrintStmt *prnt_stmt);
    void execute_expr_stmt(ExprStmt *expr_stmt);
    void execute_stmt(Stmt *stmt);
    void execute_var_stmt(VariableStmt *var_stmt);
    void execute_block_stmt(BlockStmt *block_stmt, EnvironmentTable *environment);
    void execute_if_stmt(IfStmt *if_stmt);
    
    // utility methods
    RuntimeValue evaluate_expr(Expr *expr);
  public:
    Executor(std::vector<Stmt *> &statements): statements(statements) {}

    void execute();

};