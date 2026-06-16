#include "scanner.hpp"
#include "token.hpp"
#include <iostream>

TokenType identify_two_len_token(std::string str) {
    if (str == "==") 
        return TokenType::EQUAL_EQUAL;
    else if (str == "!=")
        return TokenType::BANG_EQUAL;

    return TokenType::UNKNOWN;
}

TokenType identify_one_len_token(char ch) {
    switch (ch) {
    case '(':
        return TokenType::LEFT_PAREN;
    case ')':
        return TokenType::RIGHT_PAREN;
    case '{':
        return TokenType::LEFT_BRACE;
    case '}':
        return TokenType::RIGHT_BRACE;
    case ',':
        return TokenType::COMMA;
    case '.':
        return TokenType::DOT;
    case '-':
        return TokenType::MINUS;
    case '+':
        return TokenType::PLUS;
    case '*':
        return TokenType::STAR;
    case ';':
        return TokenType::SEMICOLON;
    case '/':
        return TokenType::SLASH;
    case '=':
        return TokenType::EQUAL;
    case '!':
        return TokenType::BANG;
    }
    return TokenType::UNKNOWN;
}

std::pair<TokenType, int> identify_token(int idx, const std::string &file_content) {
    TokenType type = TokenType::UNKNOWN;
    int add = 1;

    if (idx + 1 < file_content.size()) {
        type = identify_two_len_token(file_content.substr(idx, 2));
        add += type != TokenType::UNKNOWN;
    }
    
    // identify one character token
    if (type == TokenType::UNKNOWN) {
        type = identify_one_len_token(file_content[idx]);
    }

    return std::make_pair(type, add);
}


std::pair<std::vector<std::string>, std::vector<std::string>> scan_file(const std::vector<std::string> &file_contents) {
    std::vector<std::string> tokens;
    std::vector<std::string> lexical_errors;

    for (int idx = 0; idx < file_contents.size(); idx++) {
        int line = idx + 1;
        const std::string &file_content = file_contents[idx];
        std::cerr << line << " " << file_content << "\n";

        for (int i = 0; i < file_content.size(); ) {
            auto [token_type, add] = identify_token(i, file_content);

            const Token token = Token{file_content.substr(i, add), token_type, line};
            i += add;
            if (token_type == TokenType::UNKNOWN) {
                lexical_errors.push_back(token.to_lexical_error());
                continue;
            }
            tokens.push_back(token.to_string());
        }
    }

    // EOF file token
    tokens.push_back(Token{"", TokenType::END_OF_FILE, file_contents.size()}.to_string());
    return std::make_pair(tokens, lexical_errors);
}