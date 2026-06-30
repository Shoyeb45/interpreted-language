#pragma once 

#include <string>

struct AetherClass {
    std::string name;

    AetherClass(std::string &name)
        : name(name) {}

    friend std::ostream &operator<<(std::ostream &os, AetherClass *aether_class) {
        os << aether_class->to_string();
        return os;
    }
    std::string to_string() {
        return name;
    }
};