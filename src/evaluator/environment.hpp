#pragma once

#include "../core/runtime_value.hpp"
#include <string>
#include <unordered_map>
#include <iostream>

struct EnvironmentTable {
  private:
    std::unordered_map<std::string, RuntimeValue> table;

  public:
    void set(std::string &name, RuntimeValue value) {
        table[name] = value;
    }

    bool exists(std::string &name) {
        return table.find(name) != table.end();
    }

    RuntimeValue get(std::string &name) {
        if (exists(name)) {
            return table[name];
        }
        std::cerr << "Undeclared variable: " << name << "\n";
        std::exit(70);
    }
};