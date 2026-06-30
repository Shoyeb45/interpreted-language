#include "./interpreter.hpp"
#include "../core/aether_class.hpp"
#include "../core/callable.hpp"
#include "../exceptions/return_exception.hpp"

void Interpreter::execute() {
    for (Stmt *stmt : stmts) {
        execute_stmt(stmt);
    }
}

/*
Main Executors
*/
RuntimeValue Interpreter::evaluate_expr(Expr *expr) {
    RuntimeValue value = evaluate(expr);

    if (errors.size() > 0) {
        report_error();
        std::exit(70);
    }
    return value;
}

void Interpreter::execute_return_stmt(ReturnStmt *return_stmt) {
    RuntimeValue return_value = evaluate(return_stmt->expr);
    if (errors.size() > 0) {
        report_error();
        std::exit(70);
    }
    throw ReturnExecption(return_value);
}

void Interpreter::execute_class_stmt(ClassStmt *class_stmt) {
    std::unordered_map<std::string, std::shared_ptr<Callable>> methods;

    for (FuncStmt *stmt : class_stmt->methods) {
        methods[stmt->name.lexeme] = std::make_shared<CustomFunction>(CustomFunction{stmt, environment});
    }
  
    AetherClass aether_class(class_stmt->name.lexeme, methods);
    environment->set(class_stmt->name.lexeme, std::make_shared<AetherClass>(aether_class));
}

void Interpreter::execute_stmt(Stmt *stmt) {
    if (!stmt)
        return;

    switch (stmt->type) {
    case NodeType::EXPR_STMT: {
        execute_expr_stmt(static_cast<ExprStmt *>(stmt));
        break;
    }
    case NodeType::PRINT_STMT: {
        execute_prnt_stmt(static_cast<PrintStmt *>(stmt));
        break;
    }
    case NodeType::VARIABLE_STMT: {
        execute_var_stmt(static_cast<VariableStmt *>(stmt));
        break;
    }
    case NodeType::BLOCK_STMT: {
        execute_block_stmt(static_cast<BlockStmt *>(stmt), new EnvironmentTable(environment));
        break;
    }
    case NodeType::IF_STMT: {
        execute_if_stmt(static_cast<IfStmt *>(stmt));
        break;
    }
    case NodeType::WHILE_STMT: {
        execute_while_stmt(static_cast<WhileStmt *>(stmt));
        break;
    }
    case NodeType::FUNCTION_STMT: {
        execute_func_stmt(static_cast<FuncStmt *>(stmt));
        break;
    }
    case NodeType::RETURN_STMT: {
        execute_return_stmt(static_cast<ReturnStmt *>(stmt));
        break;
    }
    case NodeType::CLASS_STMT: {
        execute_class_stmt(static_cast<ClassStmt *>(stmt));
        break;
    }
    }
}

void Interpreter::execute_expr_stmt(ExprStmt *expr_stmt) {
    evaluate(expr_stmt->expr);

    if (errors.size() > 0) {
        report_error();
        std::exit(70);
    }
}

void Interpreter::execute_var_stmt(VariableStmt *varStmt) {
    RuntimeValue value = evaluate_expr(varStmt->expr);
    environment->set(varStmt->identifier.lexeme, value);
}

void Interpreter::execute_prnt_stmt(PrintStmt *prntStmt) {
    RuntimeValue result = evaluate(prntStmt->expr);

    if (errors.size() > 0) {
        report_error();
        std::exit(70);
    }

    std::cout << get_runtime_to_str(result) << "\n";
}

void Interpreter::execute_block_stmt(BlockStmt *block_stmt, EnvironmentTable *env) {
    EnvironmentTable *prev = environment;

    // change the environment
    environment = env;

    try {
        for (Stmt *stmt : block_stmt->stmts) {
            execute_stmt(stmt);
        }
    } catch (...) {
        environment = prev;
        throw;
    }

    // change it into global one
    environment = prev;
}

void Interpreter::execute_if_stmt(IfStmt *if_stmt) {
    RuntimeValue result = evaluate_expr(if_stmt->expr);

    if (is_truthy(result)) {
        // then execute the then_branch
        execute_stmt(if_stmt->then_branch);
    } else {
        // here we can execute else branch
        execute_stmt(if_stmt->else_branch);
    }
}

void Interpreter::execute_while_stmt(WhileStmt *while_stmt) {
    RuntimeValue evaluated_expr;
    while (is_truthy(evaluated_expr = evaluate_expr(while_stmt->condition))) {
        execute_stmt(while_stmt->body);
    }
}

void Interpreter::execute_func_stmt(FuncStmt *func_stmt) {
    CustomFunction custm_func(func_stmt, environment);

    environment->set(func_stmt->name.lexeme, std::make_shared<CustomFunction>(custm_func));
}

/*
Evaluation Code
*/

RuntimeValue Interpreter::perform_get_expr(Get *get_node) {
    RuntimeValue value = evaluate(get_node->expr);

    if (is_aether_instance(value)) {
        auto instance = get_aether_instance(value);
        return instance->get(get_node->name);
    }
    errors.push_back(get_node->name.construct_err_message("Only instances have properties."));
    report_error();
    std::exit(70);
}

RuntimeValue Interpreter::perform_set_expr(Set *set_node) {
    RuntimeValue instance = evaluate(set_node->expr);

    if (!is_aether_instance(instance)) {
        errors.push_back(set_node->name.construct_err_message("Only instances have fields."));
        report_error();
        std::exit(70);
    }

    RuntimeValue value = evaluate_expr(set_node->value);
    get_aether_instance(instance)->set(set_node->name, value);

    return value;
}

RuntimeValue Interpreter::evaluate(Expr *node) {
    if (!node)
        return nullptr;

    switch (node->type) {
    case NodeType::LITERAL:
        return static_cast<Literal *>(node)->token.get_runtime_value();
    case NodeType::GROUP:
        return evaluate(static_cast<Group *>(node)->ast_node);
    case NodeType::BINARY:
        return perform_binary_operation(static_cast<Binary *>(node));
    case NodeType::UNARY:
        return perform_unary_operation(static_cast<Unary *>(node));
    case NodeType::VARIABLE:
        return look_up_variable(static_cast<Variable *>(node));
    case NodeType::ASSIGN:
        return assign_variable(static_cast<Assign *>(node));
    case NodeType::LOGICAL:
        return perform_logical_operation(static_cast<Logical *>(node));
    case NodeType::CALL:
        return perform_fun_call(static_cast<Call *>(node));
    case NodeType::GET:
        return perform_get_expr(static_cast<Get *>(node));
    case NodeType::SET:
        return perform_set_expr(static_cast<Set *>(node));
    };
    return nullptr;
}

RuntimeValue Interpreter::assign_variable(Assign *assign) {
    RuntimeValue value = evaluate(assign->value);

    std::string &name = assign->identifier.lexeme;

    if (locals.find(assign) != locals.end()) {
        int dist = locals.at(assign);
        environment->assignAt(name, dist, value);
        return value;
    }

    global->assign(assign->identifier, value);

    return value;
}

RuntimeValue Interpreter::perform_binary_operation(Binary *binary_node) {
    RuntimeValue left_val = evaluate(binary_node->left);
    RuntimeValue right_val = evaluate(binary_node->right);

    switch (binary_node->operation.type) {
    case TokenType::STAR: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Multiplication operand can't be string", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left * right;
    }
    case TokenType::SLASH: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Division operand can't be string", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        if ((int)right == 0) {
            push_error("Division can't be performed by 0", binary_node->operation);
            return nullptr;
        }
        return left / right;
    }
    case TokenType::MINUS: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Subtraction operand can't be string", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left - right;
    }
    case TokenType::PLUS: {
        // string concatenation
        if (is_string(left_val) && is_string(right_val)) {
            return get_string(left_val) + get_string(right_val);
        }
        if (check_invalid_values(left_val, right_val)) {
            push_error("Addition operand should be number", binary_node->operation);
            return nullptr;
        }

        double left = get_number(left_val), right = get_number(right_val);
        return left + right;
    }
    case TokenType::LESS: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Operands must be a number", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left < right;
    }
    case TokenType::LESS_EQUAL: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Operands must be a number", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left <= right;
    }
    case TokenType::GREATER: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Operands must be a number", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left > right;
    }
    case TokenType::GREATER_EQUAL: {
        if (check_invalid_values(left_val, right_val)) {
            push_error("Operands must be a number", binary_node->operation);
            return nullptr;
        }
        double left = get_number(left_val), right = get_number(right_val);
        return left >= right;
    }
    case TokenType::EQUAL_EQUAL: {
        return left_val == right_val;
    }
    case TokenType::BANG_EQUAL: {
        return left_val != right_val;
    }
    }
    push_error("Operands must be a number", binary_node->operation);

    errors.push_back("Unexpected operation " + binary_node->operation.lexeme);
    return nullptr;
}

RuntimeValue Interpreter::perform_unary_operation(Unary *unary_node) {
    RuntimeValue val = evaluate(unary_node->child);

    switch (unary_node->token.type) {
    case TokenType::BANG: {
        if (is_number(val))
            return !get_number(val);
        if (is_false(val) || is_true(val))
            return !get_bool(val);
        if (is_nil(val))
            return true;

        push_error("Expected operands 'true', 'false' or 'nil'", unary_node->token);
        return nullptr;
    }
    case TokenType::MINUS:
        if (is_number(val))
            return -get_number(val);

        push_error("Expected operand of type 'number'", unary_node->token);
        return nullptr;
    }

    push_error("Unknown operand. Expected 'number', 'nil', 'true' or 'false'", unary_node->token);
    return nullptr;
}

RuntimeValue Interpreter::perform_logical_operation(Logical *logical_node) {
    // first evaluate left
    RuntimeValue left = evaluate(logical_node->left);

    if (logical_node->op.type == TokenType::OR) {
        if (is_truthy(left))
            return left;
    } else if (!is_truthy(left)) {
        return left;
    }
    return evaluate(logical_node->right);
}

RuntimeValue Interpreter::perform_fun_call(Call *fun_call_node) {
    RuntimeValue calle = evaluate(fun_call_node->calle);

    std::vector<RuntimeValue> args;

    for (Expr *expr : fun_call_node->args)
        args.push_back(evaluate(expr));

    if (!is_callable(calle)) {
        errors.push_back(fun_call_node->name.construct_err_message("Unknown value found while calling a function"));
        return nullptr;
    }

    auto function = get_callable(calle);
    if (function->arity() != args.size()) {
        errors.push_back(fun_call_node->name.construct_err_message(
            "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(args.size())));
        return nullptr;
    }
    return function->call(this, args);
}

void Interpreter::define_native_fns() {
    std::string clock = "clock";
    ClockFunction clock_func{};
    global->set(clock, std::make_shared<ClockFunction>(clock_func));
}

// some helpers:

bool Interpreter::check_invalid_values(RuntimeValue &v1, RuntimeValue &v2) {
    return is_string(v1) || is_string(v2) || is_bool(v2) || is_bool(v1) || is_nil(v1) || is_nil(v2);
}

void Interpreter::report_error() {
    for (auto &err : errors) {
        std::cerr << err << "\n";
    }
}

void Interpreter::resolve(Expr *expr, int depth) {
    locals[expr] = depth;
}

RuntimeValue Interpreter::look_up_variable(Variable *expr) {
    std::string name = expr->identifier.lexeme;
    if (locals.find(expr) != locals.end()) {
        int dist = locals.at(expr);
        return environment->getAt(name, dist);
    }

    if (global->exists(name))
        return global->get(name);

    errors.push_back(expr->identifier.construct_err_message("Undeclared variable: " + name));
    report_error();
    std::exit(70);
}