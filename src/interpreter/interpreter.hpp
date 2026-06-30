#pragma once

#include "../core/runtime_value.hpp"
#include "../evaluator/environment.hpp"
#include "../evaluator/evaluator.hpp"
#include "../executor/executor.hpp"
#include "../parser/expr.hpp"
#include "../parser/stmt.hpp"
#include <vector>

// I was not planning to define this class, but due to functions I have to define this class in order to
// get the executor

struct Interpreter {
  public:
    EnvironmentTable *environment = new EnvironmentTable();
    // always points to global scope
    EnvironmentTable *global = environment;

    std::unordered_map<Expr*, int> locals;

    std::vector<std::string> errors;
    std::vector<Stmt *> stmts;

    void execute();
    void resolve(Expr *expr, int depth);

    // Main exeuctors
    void execute_stmt(Stmt *stmt);
    void execute_if_stmt(IfStmt *if_stmt);
    void execute_var_stmt(VariableStmt *var_stmt);
    void execute_func_stmt(FuncStmt *func_stmt);
    void execute_expr_stmt(ExprStmt *expr_stmt);
    void execute_prnt_stmt(PrintStmt *prnt_stmt);
    void execute_while_stmt(WhileStmt *while_stmt);
    void execute_class_stmt(ClassStmt *class_stmt);
    void execute_block_stmt(BlockStmt *block_stmt, EnvironmentTable *environment);
    void execute_return_stmt(ReturnStmt *return_stmt);
    // Evaluate methods
    RuntimeValue evaluate(Expr *expr);
    RuntimeValue perform_binary_operation(Binary *binary_node);
    RuntimeValue perform_unary_operation(Unary *unary_node);
    RuntimeValue perform_logical_operation(Logical *logical_node);
    RuntimeValue perform_fun_call(Call *fun_call_node);

    void define_native_fns();
    bool check_invalid_values(RuntimeValue &v1, RuntimeValue &v2);
    RuntimeValue evaluate_expr(Expr *expr);
    void report_error();
    RuntimeValue look_up_variable(Variable *expr);
    RuntimeValue assign_variable(Assign *assign);

    Interpreter(std::vector<Stmt *> &stmts) {
        this->stmts = stmts;
        define_native_fns();
    }

    void push_error(std::string message, Token token) {
        errors.push_back(token.construct_err_message(message));
    }
};

