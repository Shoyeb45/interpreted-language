#pragma once

#include <string>
#include <unordered_map>

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
    
    // Literals
    STRING,
    NUMBER,
    TRUE,
    FALSE,
    NIL,
    
    IDENTIFIER,
    RESERVED_WORD,
    
    // Error token
    UNKNOWN_CHARACTER,
    STRING_UNTERMINATED,
};

inline const std::unordered_map<std::string, std::string> reserved_words = {
    {"and", "AND"},   {"class", "CLASS"}, {"else", "ELSE"},     {"false", "FALSE"},
    {"for", "FOR"},   {"fun", "FUN"},     {"if", "IF"},         {"nil", "NIL"},
    {"or", "OR"},     {"print", "PRINT"}, {"return", "RETURN"}, {"super", "SUPER"},
    {"this", "THIS"}, {"true", "TRUE"},   {"var", "VAR"},       {"while", "WHILE"}};

struct Token {
    TokenType type;
    std::string lexeme;
    std::string literal;
    std::string original_token;
    int line;

    Token(): type(TokenType::UNKNOWN_CHARACTER) {}

    Token(std::string, TokenType, int);
    Token(std::string, TokenType, std::string, std::string, int);

    std::string to_string() const;
    std::string token_type_to_string() const;
    std::string token_type_to_lexeme() const;
    std::string to_lexical_error() const;
    std::string get_literal() const;
    bool is_error() const;

    friend std::ostream& operator<<(std::ostream& os, Token token) {
        os << token.to_string();
        return os;
    }
};