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
        if (is_string(left_val) || is_string(right_val) || is_bool(right_val) || is_bool(left_val)) {
            errors.push_back(binary_node->operation.construct_err_message("Multiplication operand can't be string"));
            return nullptr;
        }
        double left = std::get<double>(left_val), right = std::get<double>(right_val);
        return left * right;
    }
    case TokenType::SLASH: {
        if (is_string(left_val) || is_string(right_val) || is_bool(right_val) || is_bool(left_val)) {
            errors.push_back(binary_node->operation.construct_err_message("Division operand can't be string "));
            return nullptr;
        }
        double left = std::get<double>(left_val), right = std::get<double>(right_val);
        if ((int)right == 0) {
            errors.push_back(binary_node->operation.construct_err_message("Division can't be performed by 0"));
            return nullptr;
        }
        return left / right;
    }
    case TokenType::MINUS: {
        if (is_string(left_val) || is_string(right_val)) {
            errors.push_back(binary_node->operation.construct_err_message("Subtraction operand can't be string: "));
            return nullptr;
        }
        double left = std::get<double>(left_val), right = std::get<double>(right_val);
        return left - right;
    }
    case TokenType::PLUS: {
        // string concatenation
        if (is_string(left_val) && is_string(right_val)) {
            return std::get<std::string>(left_val) + std::get<std::string>(right_val);
        }
        double left = std::get<double>(left_val), right = std::get<double>(right_val);
        return left + right;
    }
    case TokenType::LESS: {
        if (is_string(left_val) || is_string(right_val)) {
            errors.push_back(binary_node->operation.construct_err_message("String values can't be compared"));
            return nullptr;
        }
        double left = std::get<double>(left_val), right = std::get<double>(right_val);
        return left < right;
    }
    case TokenType::LESS_EQUAL: {
        if (is_string(left_val) || is_string(right_val)) {
            errors.push_back(binary_node->operation.construct_err_message("String values can't be compared"));
            return nullptr;
        }
        double left = std::get<double>(left_val), right = std::get<double>(right_val);
        return left <= right;
    }
    case TokenType::GREATER: {
        if (is_string(left_val) || is_string(right_val)) {
            errors.push_back(binary_node->operation.construct_err_message("String values can't be compared"));
            return nullptr;
        }
        double left = std::get<double>(left_val), right = std::get<double>(right_val);
        return left > right;
    }
    case TokenType::GREATER_EQUAL: {
        if (is_string(left_val) || is_string(right_val)) {
            errors.push_back(binary_node->operation.construct_err_message("String values can't be compared"));
            return nullptr;
        }
        double left = std::get<double>(left_val), right = std::get<double>(right_val);
        return left >= right;
    }
    case TokenType::EQUAL_EQUAL: {
        return left_val == right_val;
    }
    case TokenType::BANG_EQUAL: {
        return left_val != right_val;
    }
    }
    errors.push_back(
        binary_node->operation.construct_err_message("Unexpected operation " + binary_node->operation.lexeme));
    return nullptr;
}

RuntimeValue Evaluator::perform_unary_operation(Unary *unary_node) {
    RuntimeValue val = evaluate(unary_node->child);

    switch (unary_node->token.type) {
    case TokenType::BANG: {
        if (is_number(val))
            return !std::get<double>(val);
        if (is_false(val) || is_true(val))
            return !std::get<bool>(val);
        if (is_nil(val))
            return true;

        errors.push_back("[line " + std::to_string(unary_node->token.line) +
                         "] Expected operands 'true', 'false' or 'nil'");
        return nullptr;
    }
    case TokenType::MINUS:
        if (is_number(val))
            return -std::get<double>(val);

        errors.push_back("[line " + std::to_string(unary_node->token.line) + "] Expected operand 'number'");
        return nullptr;
    }

    errors.push_back("[line " + std::to_string(unary_node->token.line) +
                     "] Unknown operand. Expected 'number', 'nil', 'true' or 'false'");
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