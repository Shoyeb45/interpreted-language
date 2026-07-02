#pragma once

#include "../exceptions/return_exception.hpp"
#include "../interpreter/interpreter.hpp"
#include "../parser/stmt.hpp"
#include "runtime_value.hpp"
#include <chrono>
#include <string>
#include <vector>

struct Callable {
  public:
    CallableType type;
    virtual int arity() = 0;
    virtual std::string to_string() = 0;
    virtual RuntimeValue call(Interpreter *interpreter, const std::vector<RuntimeValue> &args) = 0;
    virtual ~Callable() = default;
    virtual RuntimeValue bind(std::shared_ptr<AetherInstance> instance) = 0;
};

struct ClockFunction : Callable {
  public:
    int arity();
    RuntimeValue call(Interpreter *interpreter, const std::vector<RuntimeValue> &args);
    std::string to_string();
    RuntimeValue bind(std::shared_ptr<AetherInstance> instance) {
        return nullptr;
    }
};

struct CustomFunction : Callable {
    FuncStmt *declaration;
    EnvironmentTable *closure;
    bool is_initializer;

    CustomFunction(FuncStmt *declaration, EnvironmentTable *closure, bool is_initializer = false)
        : declaration(declaration), closure(closure), is_initializer(is_initializer) {
        type = CallableType::FUNCTION;
    }

    RuntimeValue call(Interpreter *interpreter, const std::vector<RuntimeValue> &args);
    int arity();
    std::string to_string();
    RuntimeValue bind(std::shared_ptr<AetherInstance> instance);
};