#pragma once

#include <string>
#include <vector>
#include "../parser/expr.hpp"
#include "../scanner/token.hpp"
#include "../core/runtime_value.hpp"

struct Evaluator {
    Expr* root;
    std::vector<std::string> errors;
    Evaluator(Expr* root): root(root) {}
    RuntimeValue evaluate(Expr* node);
    RuntimeValue perform_binary_opration(Binary* binary_node);
    RuntimeValue perform_unary_operation(Unary *unary_node);
    void push_error(std::string message, Token &token);

public:
    std::string evaluate();
    void set_root(Expr *root) {
        this->root = root;
    }
};