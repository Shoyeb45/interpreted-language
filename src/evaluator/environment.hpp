#pragma once

#include "../core/runtime_value.hpp"
#include "../scanner/token.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

struct EnvironmentTable {
  private:
    std::unordered_map<std::string, RuntimeValue> table;
    EnvironmentTable *enclosing;

  public:
    EnvironmentTable() : enclosing(nullptr) {}
    EnvironmentTable(EnvironmentTable *enclosing) : enclosing(enclosing) {}

    void set(std::string &name, RuntimeValue value) {
        table[name] = value;
    }

    void assign(Token &name, RuntimeValue value) {
        auto it = table.find(name.lexeme);

        if (it != table.end()) {
            it->second = value;
            return;
        }

        if (enclosing) {
            enclosing->assign(name, value);
            return;
        }

        std::cerr << name.construct_err_message("Undefined variable " + name.lexeme) << "\n";

        std::exit(70);
    }

    bool exists(std::string &name) {
        if (table.find(name) != table.end())
            return true;
        if (enclosing)
            return enclosing->exists(name);
        return false;
    }

    RuntimeValue get(std::string &name) {
        auto it = table.find(name);

        if (it != table.end())
            return it->second;

        if (enclosing)
            return enclosing->get(name);

        std::cerr << "Undeclared variable: " << name << "\n";
        std::exit(70);
    }

    
    RuntimeValue getAt(std::string &name, int depth) {
        auto env = ancestor(depth);
        return env->table.at(name);
    }

    EnvironmentTable* ancestor(int depth) {
        EnvironmentTable *env = this;
        for (int i = 0; i < depth; i++) {
            env = env->enclosing;
        }
        return env;
    }

    void assignAt(std::string &name, int depth, RuntimeValue &value) {
        auto env = ancestor(depth);
        env->table.at(name) = value;
    }
};