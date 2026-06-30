#include "scanner.hpp"
#include "token.hpp"
#include <iostream>

TokenType identify_two_len_token(std::string str) {
    if (str == "==")
        return TokenType::EQUAL_EQUAL;
    else if (str == "!=")
        return TokenType::BANG_EQUAL;
    else if (str == ">=")
        return TokenType::GREATER_EQUAL;
    else if (str == "<=")
        return TokenType::LESS_EQUAL;

    return TokenType::UNKNOWN_CHARACTER;
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
    case '<':
        return TokenType::LESS;
    case '>':
        return TokenType::GREATER;
    }
    return TokenType::UNKNOWN_CHARACTER;
}

bool is_whitespace_char(char ch) {
    return ch == ' ' || ch == '\t';
}

bool isnumber(char ch) {
    return ch >= '0' && ch <= '9';
}

bool is_start_identifier(char ch) {
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_';
}


std::pair<TokenType, int> identify_token(int idx, const std::string &file_content) {
    TokenType type = TokenType::UNKNOWN_CHARACTER;
    int add = 1;

    // identifier
    if (is_start_identifier(file_content[idx])) {
        std::string word(1, file_content[idx]);

        for (int i = idx + 1; i < file_content.size(); i++) {
            if (is_start_identifier(file_content[i]) || isnumber(file_content[i])) {
                add++;
                word += file_content[i];
                continue;
            }
            break;
        }
        if (reserved_words.find(word) != reserved_words.end()) {
            type = TokenType::RESERVED_WORD;
            if (word == "true") {
                type = TokenType::TRUE;
            } else if (word == "false") {
                type = TokenType::FALSE;
            } else if (word == "nil") {
                type = TokenType::NIL;
            } else if (word == "print") {
                type = TokenType::PRINT;
            } else if (word == "var") {
                type = TokenType::VAR;
            } else if (word == "if") {
                type = TokenType::IF;
            } else if (word == "else") {
                type = TokenType::ELSE;
            } else if (word == "or") {
                type = TokenType::OR;
            } else if (word == "and") {
                type = TokenType::AND;
            } else if (word == "if") {
                type = TokenType::IF;
            } else if (word == "for") {
                type = TokenType::FOR;
            } else if (word == "while") {
                type = TokenType::WHILE;
            } else if (word == "fun") {
                type = TokenType::FUN;
            } else if (word == "return") {
                type = TokenType::RETURN;
            } else if (word == "class") {
                type = TokenType::CLASS;
            }
        } else {
            type = TokenType::IDENTIFIER;
        }
    }

    // identify number literal
    if (isnumber(file_content[idx])) {

        for (int i = idx + 1; i < file_content.size(); i++) {
            if (isnumber(file_content[i]) || file_content[i] == '.') {
                add++;
                continue;
            }
            break;
        }
        type = TokenType::NUMBER;
    }

    // identify string literal
    if (file_content[idx] == '"') {
        int j = file_content.find("\"", idx + 1);
        if (j == std::string::npos) {
            type = TokenType::STRING_UNTERMINATED;
            add = file_content.size();
        } else {
            type = TokenType::STRING;
            add = j - idx + 1;
        }
    }

    if (idx + 1 < file_content.size() && type == TokenType::UNKNOWN_CHARACTER) {
        type = identify_two_len_token(file_content.substr(idx, 2));
        add += type != TokenType::UNKNOWN_CHARACTER;
    }

    // identify one character token
    if (type == TokenType::UNKNOWN_CHARACTER) {
        type = identify_one_len_token(file_content[idx]);
    }

    return std::make_pair(type, add);
}

std::vector<Token> LexicalScanner::scan_file(const std::vector<std::string> &file_contents) {
    // std::vector<std::string> tokens;
    std::vector<Token> tokens;
    std::vector<std::string> lexical_errors;

    int prev = 0;
    for (int idx = 0; idx < file_contents.size(); idx++) {
        int line = idx + 1;
        const std::string &file_content = file_contents[idx];

        for (int i = 0 + prev; i < file_content.size();) {
            if (is_whitespace_char(file_content[i])) {
                i++;
                continue;
            }
            // identify comment and ignore everything
            if (i + 1 < file_content.size() && file_content.substr(i, 2) == "//") {
                break;
            }

            auto [token_type, add] = identify_token(i, file_content);
            
            std::string original = file_content.substr(i, add);

            std::string str_literal = "";
            
            // if we got string terminated then we may have the other " in next line
            // so we should try to find that
            if (token_type == TokenType::STRING_UNTERMINATED) {
                for (int j = idx + 1; j < file_contents.size(); j++){
                    const std::string &str = file_contents[j];
                    int k = str.find("\"", 0);

                    if (k == std::string::npos) {
                        str_literal += str + "\n";
                    } else {
                        token_type = TokenType::STRING;
                        str_literal += str.substr(0, k) + "\n";
                        idx = j - 1;
                        prev = k + 1;
                        break;
                    }
                }
            }
            // std::cout << "str_literal " << str_literal << "\n";

            original += (str_literal != "" ? "\n" + str_literal : "");
            const Token token = Token{original, token_type, line};

            if (str_literal != "") {
                tokens.push_back(token);
                break;
            } else {
                prev = 0;
            }

            i += add;
            tokens.push_back(token);
        }
    }

    // EOF file token
    tokens.push_back(Token{"", TokenType::END_OF_FILE, (int) file_contents.size()});
    return tokens;
}

bool LexicalScanner::is_lexical_error_present(const std::vector<Token> &tokens) {
    for (const Token token: tokens) {
        if (token.is_error()) return true;
    }
    return false;
}

void LexicalScanner::print_lexical_error(const std::vector<Token> &tokens) {
     for (const Token token: tokens) {
        if (token.is_error()) {
            std::cerr << token.to_lexical_error() << "\n";
        }
    }
}