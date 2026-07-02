#include "callable.hpp"
#include "aether_class.hpp"

int ClockFunction::arity() {
    return 0;
}

RuntimeValue ClockFunction::call(Interpreter *interpreter, const std::vector<RuntimeValue> &args) {
    auto sec =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    return static_cast<double>(sec);
}

std::string ClockFunction::to_string() {
    return "<func native clock>";
}


RuntimeValue CustomFunction::call(Interpreter *interpreter, const std::vector<RuntimeValue> &args) {
    EnvironmentTable *env = new EnvironmentTable(closure);

    if (arity() != args.size()) {
        // error, std::exit(70) ? maybe
        return nullptr;
    }

    for (int i = 0; i < declaration->params.size(); i++) {
        env->set(declaration->params[i].lexeme, args[i]);
    }
    try {
        interpreter->execute_block_stmt(declaration->body, env);
    } catch (const ReturnExecption &excp) {
        return excp.value;
    }

    std::string this_str = "this";
    if (is_initializer) return closure->getAt(this_str, 0);
    return nullptr;
}

int CustomFunction::arity() {
    return (int)declaration->params.size();
}

std::string CustomFunction::to_string() {
    return "<fn " + declaration->name.lexeme + ">";
}

RuntimeValue CustomFunction::bind(std::shared_ptr<AetherInstance> instance) {
    EnvironmentTable *env = new EnvironmentTable(closure);
    std::string name = "this";

    env->set(name, instance);
    
    return std::make_shared<CustomFunction>(CustomFunction(declaration, env, is_initializer));
}
