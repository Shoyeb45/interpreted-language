#pragma once

#include "../interpreter/interpreter.hpp"
#include "../parser/stmt.hpp"
#include <stack>
#include <vector>

struct Resolver {
  private:
    Interpreter *interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;

    void define(Token &name);
    void declare(Token &name);
    void end_scope();
    void begin_scope();
    void resolve_local(Expr *expr, Token &name);
    void resolve_function(FuncStmt *fun_stmt);

    void resolve(Stmt *stmt);
    void resolve_if_stmt(IfStmt *if_stmt);
    void resolve_expr_stmt(ExprStmt *expr_stmt);
    void resolve_print_stmt(PrintStmt *print_stmt);
    void resolve_block_stmt(BlockStmt *block_stmt);
    void resolve_while_stmt(WhileStmt *while_stmt);
    void resolve_return_stmt(ReturnStmt *return_stmt);
    void resolve_variable_stmt(VariableStmt *variable_stmt);
    void resolve_fun_declaration(FuncStmt *fun_stmt);

    void resolve_expr(Expr *expr);
    void resolve_grp_expr(Group *grp);
    void resolve_call_expr(Call *call);
    void resolve_unary_expr(Unary *unary);
    void resolve_assign_expr(Assign *assign);
    void resolve_binary_expr(Binary *binary);
    void resolve_logical_expr(Logical *logical);
    void resolve_variable_expr(Variable *variable);

  public:
    void resolve();
    void resolve_stmts(std::vector<Stmt *> &stmts);

    Resolver(Interpreter *interpreter) {
        this->interpreter = interpreter;
    }
};