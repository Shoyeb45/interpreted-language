#include "token.hpp"
#include <string>

Token::Token(std::string orgnal_char, TokenType type, int line) {
    this->original_token = orgnal_char;
    this->type = type;
    this->line = line;
    this->lexeme = this->token_type_to_symbol();
    this->literal = "null";
}

Token::Token(std::string original_token,TokenType type, std::string lexeme, std::string literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line), original_token(original_token) {}

std::string Token::to_string() const {
    std::string lexeme = this->lexeme;
    if (lexeme == "") {
        lexeme = this->token_type_to_symbol();
    }

    return token_type_to_string() + " " + lexeme + " " + literal;
}

std::string Token::to_lexical_error() const {
    std::string error_msg = "";

    if (type == TokenType::UNKNOWN) {
        error_msg = "[line " + std::to_string(this->line) + "] Error: Unexpected character: " + this->original_token;
    }

    return error_msg;
}

std::string Token::token_type_to_string() const {
    switch (type) {
    case TokenType::END_OF_FILE:
        return "EOF";
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:
        return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::DOT:
        return "DOT";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::SLASH:
        return "SLASH";
    case TokenType::STAR:
        return "STAR";
    case TokenType::EQUAL:
        return "EQUAL";
    case TokenType::EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case TokenType::BANG_EQUAL:
        return "BANG_EQUAL";
    case TokenType::BANG:
        return "BANG";
    case TokenType::UNKNOWN:
    default:
        return "";
    }
}

std::string Token::token_type_to_symbol() const {
    switch (type) {
    case TokenType::END_OF_FILE:
        return "";
    case TokenType::LEFT_PAREN:
        return "(";
    case TokenType::RIGHT_PAREN:
        return ")";
    case TokenType::LEFT_BRACE:
        return "{";
    case TokenType::RIGHT_BRACE:
        return "}";
    case TokenType::COMMA:
        return ",";
    case TokenType::DOT:
        return ".";
    case TokenType::MINUS:
        return "-";
    case TokenType::PLUS:
        return "+";
    case TokenType::SEMICOLON:
        return ";";
    case TokenType::SLASH:
        return "/";
    case TokenType::STAR:
        return "*";
    case TokenType::EQUAL:
        return "=";
    case TokenType::EQUAL_EQUAL:
        return "==";
    case TokenType::BANG_EQUAL:
        return "!=";
    case TokenType::BANG:
        return "!";
    default:
        return "";
    }
}