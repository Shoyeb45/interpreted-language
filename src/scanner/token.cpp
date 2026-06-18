#include "token.hpp"
#include <string>

Token::Token(std::string orgnal_char, TokenType type, int line) {
    this->original_token = orgnal_char;
    this->type = type;
    this->line = line;
    this->lexeme = this->token_type_to_lexeme();
    this->literal = this->get_literal();
}

Token::Token(std::string original_token, TokenType type, std::string lexeme, std::string literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line), original_token(original_token) {}

std::string Token::to_string() const {
    std::string lexeme = this->lexeme;
    if (lexeme == "") {
        lexeme = this->token_type_to_lexeme();
    }

    return token_type_to_string() + " " + lexeme + " " + literal;
}

std::string Token::to_lexical_error() const {
    std::string error_msg = "";

    switch (type) {
    case TokenType::UNKNOWN_CHARACTER:
        error_msg = "[line " + std::to_string(this->line) + "] Error: Unexpected character: " + this->original_token;
        break;
    case TokenType::STRING_UNTERMINATED:
        error_msg = "[line " + std::to_string(this->line) + "] Error: Unterminated string.";
        break;
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
    case TokenType::LESS:
        return "LESS";
    case TokenType::LESS_EQUAL:
        return "LESS_EQUAL";
    case TokenType::GREATER:
        return "GREATER";
    case TokenType::GREATER_EQUAL:
        return "GREATER_EQUAL";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::RESERVED_WORD:
    case TokenType::FALSE:
    case TokenType::TRUE:
    case TokenType::NIL:
        return reserved_words.at(original_token);
    case TokenType::UNKNOWN_CHARACTER:
    default:
        return "";
    }
}

std::string Token::token_type_to_lexeme() const {
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
    case TokenType::LESS:
        return "<";
    case TokenType::LESS_EQUAL:
        return "<=";
    case TokenType::GREATER:
        return ">";
    case TokenType::GREATER_EQUAL:
        return ">=";
    case TokenType::STRING:
    case TokenType::NUMBER:
    case TokenType::IDENTIFIER:
    case TokenType::RESERVED_WORD:
    case TokenType::TRUE:
    case TokenType::FALSE:
    case TokenType::NIL:
        return original_token;
    default:
        return "";
    }
}

std::string normalize_number_literal(const std::string &num) {
    std::string str_num = std::to_string(std::stod(num));

    while (!str_num.empty() && str_num.back() == '0') {
        str_num.pop_back();
    }

    if (str_num.back() == '.')
        str_num += "0";
    return str_num;
}

std::string Token::get_literal() const {
    switch (type) {
    case TokenType::STRING:
        return original_token.substr(1, original_token.size() - 2);
    case TokenType::NUMBER:
        return normalize_number_literal(original_token);
    }

    return "null";
}

bool Token::is_error() const {
    switch (type) {
    case TokenType::STRING_UNTERMINATED:
    case TokenType::UNKNOWN_CHARACTER:
        return true;
    }
    return false;
}