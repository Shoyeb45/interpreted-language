#include "runtime_value.hpp"
#include "utils.hpp"
#include "callable.hpp"
#include "aether_class.hpp"
#include <iostream>
#include <string>

bool is_number(RuntimeValue &value) {
    return std::holds_alternative<double>(value);
}

bool is_string(RuntimeValue &value) {
    return std::holds_alternative<std::string>(value);
}

bool is_truthy(RuntimeValue &value) {
    if (is_bool(value))
        return get_bool(value);

    if (is_number(value))
        return get_number(value) != 0.0;

    if (is_string(value))
        return true;

    if (is_nil(value))
        return false;

    return true;
}
bool is_bool(RuntimeValue &value) {
    return is_true(value) || is_false(value);
}

bool is_nil(RuntimeValue &value) {
    return std::holds_alternative<nullptr_t>(value);
}
bool is_true(RuntimeValue &value) {
    if (std::holds_alternative<bool>(value))
        return std::get<bool>(value);
    return false;
}

bool is_false(RuntimeValue &value) {
    if (std::holds_alternative<bool>(value))
        return !std::get<bool>(value);
    return false;
}

bool get_bool(RuntimeValue &value) {
    return std::get<bool>(value);
}

nullptr_t get_nil(RuntimeValue &value) {
    return std::get<nullptr_t>(value);
}

std::string get_string(RuntimeValue &value) {
    return std::get<std::string>(value);
}

double get_number(RuntimeValue &value) {
    return std::get<double>(value);
}

std::string get_runtime_to_str(RuntimeValue &value) {
    if (is_true(value)) {
        return "true";
    } else if (is_false(value)) {
        return "false";
    } else if (is_number(value)) {
        return normalize_number_literal(get_number(value));
    } else if (is_nil(value)) {
        return "nil";
    } else if (is_string(value)) {
        return get_string(value);
    } else if (is_callable(value)) {
        return get_callable(value)->to_string();
    } else if (is_aether_class(value)) {
        return get_aether_class(value)->to_string();
    }
    return "";
}

bool is_callable(RuntimeValue &value) {
    return std::holds_alternative<std::shared_ptr<Callable>>(value);
}

std::shared_ptr<Callable> get_callable(RuntimeValue &value) {
    return std::get<std::shared_ptr<Callable>>(value);
}

bool is_aether_class(RuntimeValue &value) {
    return std::holds_alternative<std::shared_ptr<AetherClass>>(value);
}

std::shared_ptr<AetherClass> get_aether_class(RuntimeValue &value) {
    return std::get<std::shared_ptr<AetherClass>>(value);
}