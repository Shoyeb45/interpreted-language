#pragma once

#include <variant>
#include <memory>
#include <string>

struct Callable;

using RuntimeValue = std::variant<double, bool, std::string, std::nullptr_t, std::shared_ptr<Callable>>;

bool is_number(RuntimeValue &value);
bool is_string(RuntimeValue &value);
bool is_nil(RuntimeValue &value);
bool is_truthy(RuntimeValue &value);
bool is_true(RuntimeValue &value);
bool is_false(RuntimeValue &value);
bool is_bool(RuntimeValue &value);
bool is_callable(RuntimeValue &value);
bool is_callable(RuntimeValue &value);

double get_number(RuntimeValue &value);
std::string get_string(RuntimeValue &value);
std::string get_runtime_to_str(RuntimeValue &value);
std::shared_ptr<Callable> get_callable(RuntimeValue &value);
nullptr_t get_nil(RuntimeValue &value);
bool get_bool(RuntimeValue &value);