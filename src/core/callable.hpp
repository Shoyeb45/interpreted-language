#pragma once

#include "../interpreter/interpreter.hpp"
#include "../parser/stmt.hpp"
#include "runtime_value.hpp"
#include <chrono>
#include <vector>
#include <string>

struct Callable {
  public:
    virtual int arity() = 0;
    virtual std::string to_string() = 0;
    virtual RuntimeValue call(Interpreter *interpreter, const std::vector<RuntimeValue> &args) = 0;
    virtual ~Callable() = default;
};

struct ClockFunction : Callable {
  public:
    int arity() {
        return 0;
    }

    RuntimeValue call(Interpreter *interpreter, const std::vector<RuntimeValue> &args) {
        auto sec =
            std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())
                .count();

        return static_cast<double>(sec);
    }

    std::string to_string() {
        return "<func native clock>";
    }
};

struct CustomFunction : Callable {
    FuncStmt *declaration;

    CustomFunction(FuncStmt *declaration) {
        this->declaration = declaration;
    }

    RuntimeValue call(Interpreter *interpreter, const std::vector<RuntimeValue> &args) {
        EnvironmentTable *env = new EnvironmentTable(interpreter->global);

        if (declaration->params.size() != args.size()) {
            // error, std::exit(70) ? maybe
            return nullptr;
        }

        for (int i = 0; i < declaration->params.size(); i++) {
            env->set(declaration->params[i].lexeme, args[i]);
        }
        interpreter->execute_block_stmt(declaration->body, env);
        return nullptr;
    }

    int arity() {
        return (int) declaration->params.size();
    }

    std::string to_string() {
        return "<fn " + declaration->name.lexeme + ">";
    }
};