#include "runtime_value.hpp"
#include <string>
#include <iostream>

bool is_number(RuntimeValue &value) {
    return std::holds_alternative<double>(value);
}

bool is_string(RuntimeValue &value) {
    return std::holds_alternative<std::string>(value);
}

bool is_truthy(RuntimeValue &value) {
    if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value);
    }
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