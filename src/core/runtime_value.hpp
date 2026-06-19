#pragma once

#include <variant>
#include <string>

using RuntimeValue = std::variant<double, bool, std::string, std::nullptr_t>;

bool is_number(RuntimeValue &value);
bool is_string(RuntimeValue &value);
bool is_nil(RuntimeValue &value);
bool is_truthy(RuntimeValue &value);
bool is_true(RuntimeValue &value);
bool is_false(RuntimeValue &value);
bool is_bool(RuntimeValue &value);