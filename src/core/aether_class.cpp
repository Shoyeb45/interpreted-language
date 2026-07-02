#include "aether_class.hpp"

std::string AetherInstance::to_string() {
    return aether_class->name + " instance";
}

RuntimeValue AetherInstance::get(Token &name) {
    if (fields.find(name.lexeme) != fields.end()) {
        return fields.at(name.lexeme);
    }

    auto method = aether_class->find_method(name.lexeme);

    if (method) {
        return method->bind(shared_from_this());
    }

    std::cerr << name.construct_err_message("Undefined property '" + name.lexeme + "'.") << "\n";
    std::exit(70);
}

void AetherInstance::set(Token &name, RuntimeValue &val) {
    fields[name.lexeme] = val;
}

std::string AetherClass::to_string() {
    return name;
}

std::shared_ptr<Callable> AetherClass::find_method(std::string &name) {
    if (methods.find(name) != methods.end()) {
        return methods.at(name);
    }

    return nullptr;
}

int AetherClass::arity() {
    std::string init = "init";

    auto initializer = find_method(init);
    if (initializer) return initializer->arity();
    return 0;
}

RuntimeValue AetherClass::call(Interpreter *interpreter, const std::vector<RuntimeValue> &args) {
    AetherInstance instance(this);
    std::string _init = "init";
    auto init = find_method(_init);

    auto ptr = std::make_shared<AetherInstance>(instance);
    if (init) {
        RuntimeValue constructor = init->bind(ptr);
        if (is_callable(constructor)) {
            get_callable(constructor)->call(interpreter, args);
        }
    }

    return ptr;
}
