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

ASTNode *Parser::expression() {
    return term();
}

ASTNode *Parser::primary() {
    if (check(TokenType::NUMBER) || check(TokenType::TRUE) || check(TokenType::FALSE) || check(TokenType::NIL) ||
        check(TokenType::STRING)) {
        return new Literal(advance());
    }

    if (match(TokenType::LEFT_PAREN)) {
        // '(' consumed
        // create new one for inside of this brackets

        // create node for the group
        ASTNode *expr = expression();
        ASTNode *grp_node = new Group(expr);

        if (!match(TokenType::RIGHT_PAREN)) {
            std::cerr << "Expected ')'\n";
            return nullptr;
        }
        return grp_node;
    }

    return nullptr;
}

ASTNode *Parser::factor() {
    ASTNode *expr = unary();

    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        Token op = advance();
        ASTNode *right = unary();

        expr = new Binary(expr, right, op);
    }

    return expr;
}

ASTNode *Parser::unary() {
    if (check(TokenType::MINUS) || check(TokenType::BANG)) {
        Token op = advance();
        ASTNode *right = unary();

        return new Unary(op, right);
    }

    return primary();
}

ASTNode *Parser::term() {
    ASTNode *expr = factor();

    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        Token op = advance();
        ASTNode *right = factor();

        expr = new Binary(expr, right, op);
    }

    return expr;
}

void Parser::visualize() {
    if (!root)
        return;
    std::string ast = root->visualize();
    std::cout << trim(ast) << "\n";
}
