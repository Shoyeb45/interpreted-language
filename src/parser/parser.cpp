#include "parser.hpp"
#include "../core/utils.hpp"
#include <iostream>

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
    return equality();
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

    errors.push_back("[line " + std::to_string(peek().line) + "] Error at ')': Expect expression.");
    return nullptr;
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

    return primary();
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
        statements.push_back(statement());
    }
    return statements;
}

Stmt *Parser::var_stmt() {
    Token identifier = consume(TokenType::IDENTIFIER, "Expected name of the variable");
    consume(TokenType::EQUAL, "Expected assignment operator '='");

    Expr *expr = expression();
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
    return nullptr;
}

Stmt *Parser::prnt_stmt() {
    Expr *expr = expression();
    if (match(TokenType::SEMICOLON)) {
        return new PrintStmt(expr);
    }
    errors.push_back(previous().construct_err_message("Expected ;"));
    return nullptr;
}

Stmt *Parser::statement() {
    if (match(TokenType::PRINT)) {
        return prnt_stmt();
    }
    if (match(TokenType::VAR)) {
        return var_stmt();
    }

    return expression_stmt();
}
