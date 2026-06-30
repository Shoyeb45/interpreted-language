#include "resolver.hpp"
#include "../parser/node_types.hpp"
#include <iostream>

// helpers
void Resolver::declare(Token &name) {
    if (scopes.empty())
        return;

    std::unordered_map<std::string, bool> &scope = scopes.back();
    if (scope.find(name.lexeme) != scope.end()) {
        std::cerr << name.construct_err_message("Variable '" + name.lexeme + "' is already declared in this scope.")
                  << "\n";
        std::exit(65);
    }

    scope[name.lexeme] = false;
}

void Resolver::define(Token &name) {
    if (scopes.empty())
        return;

    std::unordered_map<std::string, bool> &scope = scopes.back();
    scope[name.lexeme] = true;
}

void Resolver::resolve_local(Expr *expr, Token &name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].find(name.lexeme) != scopes[i].end()) {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::begin_scope() {
    scopes.emplace_back();
}

void Resolver::end_scope() {
    if (!scopes.empty())
        scopes.pop_back();
}

// Statement resolvers
void Resolver::resolve_function(FuncStmt *fun_stmt, std::string function_type) {
    begin_scope();
    for (Token &param : fun_stmt->params) {
        declare(param);
        define(param);
    }
    resolve_stmts(fun_stmt->body->stmts);
    end_scope();
}

void Resolver::resolve_block_stmt(BlockStmt *block_stmt) {
    begin_scope();
    resolve_stmts(block_stmt->stmts);
    end_scope();
}

void Resolver::resolve_variable_stmt(VariableStmt *variable_stmt) {
    declare(variable_stmt->identifier);

    if (variable_stmt->expr) {
        resolve_expr(variable_stmt->expr);
    }

    define(variable_stmt->identifier);
}

void Resolver::resolve_fun_declaration(FuncStmt *fun_stmt) {
    declare(fun_stmt->name);
    define(fun_stmt->name);

    resolve_function(fun_stmt, "function");
}

void Resolver::resolve_expr_stmt(ExprStmt *expr_stmt) {
    resolve_expr(expr_stmt->expr);
}

void Resolver::resolve_if_stmt(IfStmt *if_stmt) {
    resolve_expr(if_stmt->expr);
    resolve(if_stmt->then_branch);

    if (if_stmt->else_branch)
        resolve(if_stmt->else_branch);
}

void Resolver::resolve_print_stmt(PrintStmt *print_stmt) {
    resolve_expr(print_stmt->expr);
}

void Resolver::resolve_return_stmt(ReturnStmt *return_stmt) {
    if (return_stmt->expr)
        resolve_expr(return_stmt->expr);
}

void Resolver::resolve_while_stmt(WhileStmt *while_stmt) {
    resolve_expr(while_stmt->condition);
    resolve(while_stmt->body);
}

void Resolver::resolve_class_declaration(ClassStmt *class_stmt) {
    declare(class_stmt->name);
    define(class_stmt->name);
    for (FuncStmt *stmt : class_stmt->methods) {
        resolve_function(stmt, "method");
    }
}

void Resolver::resolve(Stmt *stmt) {
    switch (stmt->type) {
    case NodeType::BLOCK_STMT:
        resolve_block_stmt(static_cast<BlockStmt *>(stmt));
        break;
    case NodeType::VARIABLE_STMT:
        resolve_variable_stmt(static_cast<VariableStmt *>(stmt));
        break;
    case NodeType::EXPR_STMT:
        resolve_expr_stmt(static_cast<ExprStmt *>(stmt));
        break;
    case NodeType::FUNCTION_STMT:
        resolve_fun_declaration(static_cast<FuncStmt *>(stmt));
        break;
    case NodeType::IF_STMT:
        resolve_if_stmt(static_cast<IfStmt *>(stmt));
        break;
    case NodeType::PRINT_STMT:
        resolve_print_stmt(static_cast<PrintStmt *>(stmt));
        break;
    case NodeType::RETURN_STMT:
        resolve_return_stmt(static_cast<ReturnStmt *>(stmt));
        break;
    case NodeType::WHILE_STMT:
        resolve_while_stmt(static_cast<WhileStmt *>(stmt));
        break;
    case NodeType::CLASS_STMT:
        resolve_class_declaration(static_cast<ClassStmt *>(stmt));
        break;
    }
}

// expression resolvers

void Resolver::resolve_assign_expr(Assign *assign) {
    resolve_expr(assign->value);
    resolve_local(assign, assign->identifier);
}

void Resolver::resolve_variable_expr(Variable *variable) {
    if (!scopes.empty()) {
        auto &scope = scopes.back();
        if (scope.find(variable->identifier.lexeme) != scope.end() && !scope.at(variable->identifier.lexeme)) {
            std::cerr << variable->identifier.construct_err_message("Can't read local variable in its own initializer.")
                      << "\n";
            std::exit(65);
        }
    }
    resolve_local(variable, variable->identifier);
}

void Resolver::resolve_binary_expr(Binary *binary) {
    resolve_expr(binary->left);
    resolve_expr(binary->right);
}

void Resolver::resolve_call_expr(Call *call) {
    resolve_expr(call->calle);

    for (Expr *arg : call->args) {
        resolve_expr(arg);
    }
}

void Resolver::resolve_grp_expr(Group *grp) {
    resolve_expr(grp->ast_node);
}

void Resolver::resolve_logical_expr(Logical *logical) {
    resolve_expr(logical->left);
    resolve_expr(logical->right);
}

void Resolver::resolve_unary_expr(Unary *unary) {
    resolve_expr(unary->child);
}

void Resolver::resolve_get_expr(Get *get) {
    resolve_expr(get->expr);
}

void Resolver::resolve_set_expr(Set *set) {
    resolve_expr(set->expr);
    resolve_expr(set->value);
}

void Resolver::resolve_expr(Expr *expr) {
    switch (expr->type) {
    case NodeType::VARIABLE:
        resolve_variable_expr(static_cast<Variable *>(expr));
        break;
    case NodeType::ASSIGN:
        resolve_assign_expr(static_cast<Assign *>(expr));
        break;
    case NodeType::BINARY:
        resolve_binary_expr(static_cast<Binary *>(expr));
        break;
    case NodeType::CALL:
        resolve_call_expr(static_cast<Call *>(expr));
        break;
    case NodeType::GROUP:
        resolve_grp_expr(static_cast<Group *>(expr));
        break;
    case NodeType::LOGICAL:
        resolve_logical_expr(static_cast<Logical *>(expr));
        break;
    case NodeType::UNARY:
        resolve_unary_expr(static_cast<Unary *>(expr));
        break;
    case NodeType::GET:
        resolve_get_expr(static_cast<Get *>(expr));
        break;
    case NodeType::SET:
        resolve_set_expr(static_cast<Set *>(expr));
        break;
    }
}

// public exposed function
void Resolver::resolve_stmts(std::vector<Stmt *> &stmts) {
    for (Stmt *stmt : stmts) {
        resolve(stmt);
    }
}

void Resolver::resolve() {
    resolve_stmts(interpreter->stmts);
}