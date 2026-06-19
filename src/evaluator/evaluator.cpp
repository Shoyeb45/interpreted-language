#include "evaluator.hpp"
#include "../core/runtime_value.hpp"
#include "../core/utils.hpp"
#include <string>

std::string Evaluator::evaluate() {
    if (!root)
        return "";
    RuntimeValue result = this->evaluate(root);

    if (is_true(result)) {
        return "true";
    } else if (is_false(result)) {
        return "false";
    } else if (is_number(result)) {
        return normalize_number_literal(std::get<double>(result));
    } else if (is_nil(result)) {
        return "nil";
    } else if (is_string(result)) {
        return std::get<std::string>(result);
    }
    return "";
}


RuntimeValue Evaluator::perform_binary_opration(Binary *binary_node) {
    RuntimeValue left_val = evaluate(binary_node->left);
    RuntimeValue right_val = evaluate(binary_node->right);

    switch (binary_node->operation.type) {
        case TokenType::STAR: {
            if (is_string(left_val) || is_string(right_val)) {
                // error
                return nullptr;
            }
            double left = std::get<double>(left_val), right = std::get<double>(right_val);
            return left * right;
        }
        case TokenType::SLASH: {
            if (is_string(left_val) || is_string(right_val)) {
                // error
                return nullptr;
            }
            double left = std::get<double>(left_val), right = std::get<double>(right_val);
            if ((int) right == 0) {
                // error and improve comparison
                return nullptr;
            }
            return left / right;

        }
        case TokenType::MINUS: {
            
        }
        case TokenType::PLUS: {

        }
    }
    return nullptr;
}

RuntimeValue Evaluator::perform_unary_operation(Unary *unary_node) {
    RuntimeValue val = evaluate(unary_node->child);

    switch (unary_node->token.type) {
        case TokenType::BANG: {
            if (is_string(val)) {
                // error hanlding
                return nullptr;
            }
            if (is_number(val)) {
                return !std::get<double>(val);
            }
            if (is_false(val) || is_true(val)) 
                return !std::get<bool>(val);
            if (is_nil(val))
                return true;
            // error handling
            return nullptr;
        }
        case TokenType::MINUS:
            if (is_number(val))
                return -std::get<double>(val);
            // error handling
            return nullptr;
    }
    // potential error handling
    return nullptr;
}

RuntimeValue Evaluator::evaluate(ASTNode *node) {
    if (!node)
        return nullptr;

    switch (node->type) {
    case NodeType::LITERAL:
        return static_cast<Literal *>(node)->token.get_runtime_value();
    case NodeType::GROUP:
        return evaluate(static_cast<Group *>(node)->ast_node);
    case NodeType::BINARY: {
        Binary *binary_node = static_cast<Binary *>(node);
        return perform_binary_opration(binary_node);
    }
    case NodeType::UNARY: {
        Unary *unary_node = static_cast<Unary *>(node);
        return perform_unary_operation(unary_node);
    }
    };
    return nullptr;
}