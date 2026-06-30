#pragma once 

#include <string>
#include "callable.hpp"
#include <unordered_map>

struct AetherInstance {
    AetherClass *aether_class;
    std::unordered_map<std::string, RuntimeValue> fields;

    AetherInstance(AetherClass *aether_class)
        : aether_class(aether_class) {}

    std::string to_string();
    RuntimeValue get(Token &name);
    void set(Token &name, RuntimeValue &value);
};

struct AetherClass : Callable {
    std::string name;

    AetherClass(std::string &name)
        : name(name) {}

    std::string to_string();

    int arity();
    RuntimeValue call(Interpreter *interpreter, const std::vector<RuntimeValue> &args);
};

