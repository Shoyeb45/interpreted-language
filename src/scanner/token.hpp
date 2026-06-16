#pragma once

#include <string>

enum class TokenType {
    END_OF_FILE,
    // 1 char token
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    EQUAL,
    BANG,
    LESS,
    GREATER,
    
    // 2 char token
    EQUAL_EQUAL,
    BANG_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    
    // 
    STRING,
    STRING_UNTERMINATED,
    UNKNOWN_CHARACTER


};

struct Token {
    TokenType type;
    std::string lexeme;
    std::string literal;
    std::string original_token;
    int line;
    // int column;

    Token(std::string, TokenType, int);
    Token(std::string, TokenType, std::string, std::string, int);

    std::string to_string() const;
    std::string token_type_to_string() const;
    std::string token_type_to_lexeme() const;
    std::string to_lexical_error() const;
    std::string get_literal() const;
};