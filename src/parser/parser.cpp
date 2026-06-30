#include "parser.hpp"
#include "../core/utils.hpp"
#include <iostream>
#include <vector>

Token Parser::peek() {
    return tokens[curr_index];
}

Token Parser::previous() {
    if (curr_index - 1 >= 0)
        return tokens[curr_index - 1];
    return tokens[0];
}

Token Parser::advance() {
    if (!is_at_end())
        curr_index++;
    return previous();
}

bool Parser::is_at_end() {
    return tokens[curr_index].type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) {
    if (peek().type == type) {
        return true;
    }
    return false;
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, std::string message) {
    if (check(type))
        return advance();
    errors.push_back(message);
    return Token{};
}

Expr *Parser::expression() {
    return assignment();
}

Expr *Parser::primary() {
    if (check(TokenType::NUMBER) || check(TokenType::TRUE) || check(TokenType::FALSE) || check(TokenType::NIL) ||
        check(TokenType::STRING)) {
        return new Literal(advance());
    }

    if (match(TokenType::LEFT_PAREN)) {
        // '(' consumed
        // create new one for inside of this brackets

        // create node for the group
        Expr *expr = expression();
        if (!expr) {
            std::cerr << "error" << "\n";
            return nullptr;
        }
        Expr *grp_node = new Group(expr);

        if (!match(TokenType::RIGHT_PAREN)) {
            errors.push_back(peek().construct_err_message("Expected ')'"));
            return nullptr;
        }
        return grp_node;
    }

    if (check(TokenType::IDENTIFIER)) {
        return new Variable(advance());
    }

    errors.push_back(peek().construct_err_message("Error at ')': Expect expression."));
    return nullptr;
}

Expr *Parser::call() {
    Expr *expr = primary();
    Token name = previous();

    while (!is_at_end()) {
        // arg starting
        if (match(TokenType::LEFT_PAREN)) {
            expr = finish_call(expr, name);
        } else if (match(TokenType::DOT)) {
            Token name = consume(TokenType::IDENTIFIER, "Expected property name after '.'");
            expr = new Get(expr, name);
        } else {
            break;
        }
    }
    return expr;
}

Expr *Parser::finish_call(Expr *calle, Token &name) {
    std::vector<Expr *> arguments;
    // if we have args: ( arg1, arg2, ... )
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }

    consume(TokenType::RIGHT_PAREN, previous().construct_err_message("Expected ')' at the argument ending"));

    return new Call(name, calle, arguments);
}

Expr *Parser::factor() {
    Expr *expr = unary();

    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        Token op = advance();
        Expr *right = unary();

        expr = new Binary(expr, right, op);
    }

    return expr;
}

Expr *Parser::unary() {
    if (check(TokenType::MINUS) || check(TokenType::BANG)) {
        Token op = advance();
        Expr *right = unary();

        return new Unary(op, right);
    }

    return call();
}

Expr *Parser::comparison() {
    Expr *expr = term();

    while (check(TokenType::LESS) || check(TokenType::LESS_EQUAL) || check(TokenType::GREATER) ||
           check(TokenType::GREATER_EQUAL)) {
        Token op = advance();
        Expr *right = term();

        expr = new Binary(expr, right, op);
    }

    return expr;
}

Expr *Parser::or_expr() {
    Expr *expr = and_expr();

    while (match(TokenType::OR)) {
        Token op = previous();
        Expr *right = and_expr();
        expr = new Logical(expr, right, op);
    }

    return expr;
}

Expr *Parser::and_expr() {
    Expr *expr = equality();

    while (match(TokenType::AND)) {
        Token op = previous();
        Expr *right = equality();
        expr = new Logical(expr, right, op);
    }

    return expr;
}

Expr *Parser::assignment() {
    Expr *expr = or_expr();

    if (check(TokenType::EQUAL)) {
        Token equals = advance();
        // again call assignment because we have another assignment
        // var x = y = 1;
        //     ^   ^
        Expr *value = assignment();
        if (expr->type == NodeType::VARIABLE) {
            Variable *var = static_cast<Variable *>(expr);
            return new Assign(var->identifier, value);
        } else if (expr->type == NodeType::GET) {
            Get *get = static_cast<Get *>(expr);
            return new Set(get->expr, value, get->name);
        }
        errors.push_back(equals.construct_err_message("Invalid assignment operation."));
    }
    return expr;
}

Expr *Parser::equality() {
    Expr *expr = comparison();

    while (check(TokenType::EQUAL_EQUAL) || check(TokenType::BANG_EQUAL)) {
        Token op = advance();
        Expr *right = comparison();

        expr = new Binary(expr, right, op);
    }

    return expr;
}

Expr *Parser::term() {
    Expr *expr = factor();

    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        Token op = advance();
        Expr *right = factor();

        expr = new Binary(expr, right, op);
    }

    return expr;
}

void Parser::visualize() {
    if (!root) {
        for (Stmt *stmt : statements) {
            std::cout << trim(stmt->visualize()) << "\n";
        }
        return;
    }
    std::string ast = root->visualize();
    std::cout << trim(ast) << "\n";
}

void Parser::report_error() {
    if (!is_error())
        return;
    for (int i = 0; i < errors.size(); i++) {
        std::cerr << errors[i] << "\n";
    }
}

std::vector<Stmt *> Parser::parse_stmt() {
    while (!is_at_end()) {
        // std::cout << peek() << "\n";
        statements.push_back(statement());
    }
    return statements;
}

Stmt *Parser::var_stmt() {
    Token identifier =
        consume(TokenType::IDENTIFIER, previous().construct_err_message("Expected name of the variable"));

    Expr *expr;
    if (match(TokenType::EQUAL)) {
        expr = expression();
    } else {
        expr = new Literal(Token{"nil", TokenType::NIL, -1});
    }

    if (expr == nullptr) {
        errors.push_back(previous().construct_err_message("Undeclared variable: " + identifier.lexeme));
    }

    consume(TokenType::SEMICOLON, previous().construct_err_message("Expected ;"));
    return new VariableStmt(expr, identifier);
}

Stmt *Parser::expression_stmt() {
    Expr *expr = expression();
    if (match(TokenType::SEMICOLON)) {
        return new ExprStmt(expr);
    }

    // error
    errors.push_back(previous().construct_err_message("Expected ;"));
    synchronize();
    return nullptr;
}

Stmt *Parser::prnt_stmt() {
    Expr *expr = expression();

    // for assignment inside print
    if (match(TokenType::EQUAL)) {
        expr = expression();
    }
    if (match(TokenType::SEMICOLON)) {
        return new PrintStmt(expr);
    }

    errors.push_back(previous().construct_err_message("Expected ;"));
    return nullptr;
}

std::vector<Stmt *> Parser::block() {
    std::vector<Stmt *> stmts;

    while (!check(TokenType::RIGHT_BRACE) && !is_at_end()) {
        stmts.push_back(statement());
    }
    return stmts;
}

Stmt *Parser::block_stmt() {
    std::vector<Stmt *> stmts = block();

    if (match(TokenType::RIGHT_BRACE))
        return new BlockStmt(stmts);

    errors.push_back(previous().construct_err_message("Expected '}'"));
    return nullptr;
}

Stmt *Parser::if_stmt() {
    // we'll support both format with paren and without it also
    if (match(TokenType::LEFT_PAREN)) {
        // with paren
        Expr *expr = expression();
        consume(TokenType::RIGHT_PAREN, previous().construct_err_message("Expected ')"));

        Stmt *then_branch = statement();
        uninitalize_var_error(then_branch);
        Stmt *else_branch = nullptr;

        if (match(TokenType::ELSE)) {
            else_branch = statement();
            uninitalize_var_error(else_branch);
        }

        return new IfStmt(expr, then_branch, else_branch);
    }

    // this will handle such case
    // if x + y > 12 { ... }

    Expr *expr = expression();
    Stmt *then_branch = statement();
    uninitalize_var_error(then_branch);
    Stmt *else_branch = nullptr;

    if (match(TokenType::ELSE)) {
        else_branch = statement();
        uninitalize_var_error(else_branch);
    }

    return new IfStmt(expr, then_branch, else_branch);
}

Stmt *Parser::while_stmt() {
    // in while also we'll maintian both syntax
    // while (cond) or while cond <- both valid

    if (match(TokenType::LEFT_PAREN)) {
        Expr *condition = expression();

        consume(TokenType::RIGHT_PAREN, previous().construct_err_message("Expected ')'"));
        Stmt *body = statement();

        return new WhileStmt(condition, body);
    }

    Expr *condition = expression();
    Stmt *body = statement();
    return new WhileStmt(condition, body);
}

void Parser::uninitalize_var_error(Stmt *stmt) {
    if (stmt && stmt->type == NodeType::VARIABLE_STMT) {
        VariableStmt *var = static_cast<VariableStmt *>(stmt);

        if (var && var->expr && var->expr->type == NodeType::LITERAL) {
            Literal *literal = static_cast<Literal *>(var->expr);

            // we should not assign any uninitialize variables
            if (literal && literal->token.type == TokenType::NIL && literal->token.line == -1) {
                errors.push_back(previous().construct_err_message("Error in var; Expected expression."));
            }
        }
    }
}

// for (init; condi; update) { .. body .. }
Stmt *Parser::for_stmt() {
    consume(TokenType::LEFT_PAREN, previous().construct_err_message("Expected '('"));
    Stmt *initializer = nullptr;

    // empty initializer
    if (match(TokenType::SEMICOLON)) {
        initializer = nullptr;
    }
    // intializer with var
    else if (match(TokenType::VAR)) {
        initializer = var_stmt();
    }
    // initializer without var
    else {
        initializer = expression_stmt();
        // std::cout << initializer << "\n";
        if (!initializer) {
            errors.push_back(previous().construct_err_message("Error: Expected expression at {"));
            advance();  // {
            advance();  // }
            advance();  // ;
        }
    }

    Expr *condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
        if (!condition) {
            errors.push_back(previous().construct_err_message("Error: Expected expression at {"));
            advance();
            advance();
        }
    }
    consume(TokenType::SEMICOLON, previous().construct_err_message("Expected ';', after for loop condition"));

    Expr *update = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        update = expression();
        if (!update) {
            errors.push_back(previous().construct_err_message("Error: Expected expression at {"));
            advance();
            advance();
        }
    }
    consume(TokenType::RIGHT_PAREN, previous().construct_err_message("Expected ')' after for clauses."));

    Stmt *body = statement();
    uninitalize_var_error(body);

    // we'll merge the update part in the body itself, because it's the same thing
    if (update) {
        std::vector<Stmt *> stmts = {body, new ExprStmt(update)};
        body = new BlockStmt(stmts);
    }

    // if the no condition provided then by default provide true
    if (!condition)
        condition = new Literal(Token{"true", TokenType::TRUE, -1});

    body = new WhileStmt(condition, body);

    // initializer will once  before the condition or the main body
    if (initializer) {
        std::vector<Stmt *> stmts = {initializer, body};
        body = new BlockStmt(stmts);
    }
    return body;
}

Stmt *Parser::function_stmt(std::string kind) {
    // fun name() {
    //    ...
    // }
    Token identifier = consume(TokenType::IDENTIFIER, previous().construct_err_message("Expected " + kind + " name."));

    // Left paren
    consume(TokenType::LEFT_PAREN, previous().construct_err_message("Expect '(' after " + kind + " name."));

    std::vector<Token> params;
    // parse arguments
    if (!check(TokenType::RIGHT_PAREN)) {
        // we need to parse parameters now

        do {
            if (params.size() >= 255)
                errors.push_back(previous().construct_err_message("Can't have more than 255 parameters."));
            params.push_back(consume(TokenType::IDENTIFIER, previous().construct_err_message("Expect parameter name")));
        } while (match(TokenType::COMMA));
    }

    consume(TokenType::RIGHT_PAREN, previous().construct_err_message("Expect ')' after parameters"));

    // Now function body
    consume(TokenType::LEFT_BRACE, previous().construct_err_message("Expect '{' after " + kind + " parameters."));

    fun_depth++;
    BlockStmt *body = (BlockStmt *)block_stmt();
    fun_depth--;

    return new FuncStmt(identifier, params, body);
}

Stmt *Parser::return_stmt() {
    if (fun_depth == 0) {
        errors.push_back(previous().construct_err_message("Can't return from top-level code."));
        return nullptr;
    }
    Expr *expr = nullptr;
    if (!check(TokenType::SEMICOLON))
        expr = expression();

    consume(TokenType::SEMICOLON, previous().construct_err_message("Expected ';' at end of return statement"));
    return new ReturnStmt(expr);
}

Stmt *Parser::class_stmt() {
    Token name = consume(TokenType::IDENTIFIER, peek().construct_err_message("Expect class name."));

    consume(TokenType::LEFT_BRACE, previous().construct_err_message("Expect '{' before class body."));

    std::vector<FuncStmt *> methods;

    while (!check(TokenType::RIGHT_BRACE) && !is_at_end()) {
        methods.push_back((FuncStmt *)function_stmt("method"));
    }
    consume(TokenType::RIGHT_BRACE, previous().construct_err_message("Expect '}' after class body."));

    return new ClassStmt(name, methods);
}

Stmt *Parser::statement() {
    if (match(TokenType::PRINT)) {
        return prnt_stmt();
    }
    if (match(TokenType::VAR)) {
        return var_stmt();
    }
    if (match(TokenType::LEFT_BRACE)) {
        return block_stmt();
    }
    if (match(TokenType::IF)) {
        return if_stmt();
    }
    if (match(TokenType::WHILE)) {
        return while_stmt();
    }
    if (match(TokenType::FOR)) {
        return for_stmt();
    }
    if (match(TokenType::FUN)) {
        return function_stmt("function");
    }
    if (match(TokenType::RETURN)) {
        return return_stmt();
    }
    if (match(TokenType::CLASS)) {
        return class_stmt();
    }

    return expression_stmt();
}

void Parser::synchronize() {
    advance();

    while (!is_at_end()) {
        if (previous().type == TokenType::SEMICOLON)
            return;

        switch (peek().type) {
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::RETURN:
        case TokenType::PRINT:
            return;
        default:
            break;
        }

        advance();
    }
}