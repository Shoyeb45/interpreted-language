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
        return normalize_number_literal(get_number(result));
    } else if (is_nil(result)) {
        return "nil";
    } else if (is_string(result)) {
        return get_string(result);
    }
    return "";
}

bool check_invalid_values(RuntimeValue &v1, RuntimeValue &v2) {
    return is_string(v1) || is_string(v2) || is_bool(v2) || is_bool(v1) || is_nil(v1) || is_nil(v2);
}

void Evaluator::push_error(std::string message, Token &token) {
    errors.push_back(token.construct_err_message(message));
}

RuntimeValue Evaluator::perform_binary_opration(Binary *binary_node) {
    RuntimeValue left_val = evaluate(binary_node->left);
    RuntimeValue right_val = evaluate(binary_node->right);

    switch (binary_node->operation.type) {
    case TokenType::STAR: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Multiplication operand can't be string", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left * right;
    }
    case TokenType::SLASH: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Division operand can't be string", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        if ((int)right == 0) {
            push_error("Division can't be performed by 0", binary_node->operation);
            return nullptr;
        }
        return left / right;
    }
    case TokenType::MINUS: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Subtraction operand can't be string", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left - right;
    }
    case TokenType::PLUS: {
        // string concatenation
        if (is_string(left_val) && is_string(right_val)) {
            return get_string(left_val) + get_string(right_val);
        }
        if (check_invalid_values(left_val, right_val)) {
            push_error("Addition operand should be number", binary_node->operation);
            return nullptr;
        }

        double left = get_number(left_val), right = get_number(right_val);
        return left + right;
    }
    case TokenType::LESS: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Operands must be a number", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left < right;
    }
    case TokenType::LESS_EQUAL: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Operands must be a number", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left <= right;
    }
    case TokenType::GREATER: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Operands must be a number", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left > right;
    }
    case TokenType::GREATER_EQUAL: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Operands must be a number", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left >= right;
    }
    case TokenType::EQUAL_EQUAL: {
        return left_val == right_val;
    }
    case TokenType::BANG_EQUAL: {
        return left_val != right_val;
    }
    }
    push_error("Operands must be a number", binary_node->operation);

    errors.push_back("Unexpected operation " + binary_node->operation.lexeme);
    return nullptr;
}

RuntimeValue Evaluator::perform_unary_operation(Unary *unary_node) {
    RuntimeValue val = evaluate(unary_node->child);

    switch (unary_node->token.type) {
    case TokenType::BANG: {
        if (is_number(val))
            return !get_number(val);
        if (is_false(val) || is_true(val))
            return !get_bool(val);
        if (is_nil(val))
            return true;

        push_error("Expected operands 'true', 'false' or 'nil'", unary_node->token);
        return nullptr;
    }
    case TokenType::MINUS:
        if (is_number(val))
            return -get_number(val);

        push_error("Expected operand of type 'number'", unary_node->token);
        return nullptr;
    }

    push_error("Unknown operand. Expected 'number', 'nil', 'true' or 'false'", unary_node->token);
    return nullptr;
}

RuntimeValue Evaluator::evaluate(Expr *node) {
    if (!node)
        return nullptr;

    switch (node->type) {
    case NodeType::LITERAL:
        return static_cast<Literal *>(node)->token.get_runtime_value();
    case NodeType::GROUP:
        return evaluate(static_cast<Group *>(node)->ast_node);
    case NodeType::BINARY:
        return perform_binary_opration(static_cast<Binary *>(node));
    case NodeType::UNARY: {
        return perform_unary_operation(static_cast<Unary *>(node));
    }
    };
    return nullptr;
}