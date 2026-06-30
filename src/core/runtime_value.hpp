#pragma once

#include <memory>
#include <string>
#include <variant>

struct Callable;
struct AetherClass;

using RuntimeValue =
    std::variant<double, bool, std::string, std::nullptr_t, std::shared_ptr<Callable>, std::shared_ptr<AetherClass>>;

bool is_number(RuntimeValue &value);
bool is_string(RuntimeValue &value);
bool is_nil(RuntimeValue &value);
bool is_truthy(RuntimeValue &value);
bool is_true(RuntimeValue &value);
bool is_false(RuntimeValue &value);
bool is_bool(RuntimeValue &value);
bool is_callable(RuntimeValue &value);
bool is_aether_class(RuntimeValue &value);

bool get_bool(RuntimeValue &value);
std::string get_string(RuntimeValue &value);
double get_number(RuntimeValue &value);
nullptr_t get_nil(RuntimeValue &value);
std::string get_runtime_to_str(RuntimeValue &value);
std::shared_ptr<Callable> get_callable(RuntimeValue &value);
std::shared_ptr<AetherClass> get_aether_class(RuntimeValue &value);