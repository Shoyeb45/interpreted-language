#pragma once
#include "expr.hpp"
#include "stmt.hpp"
#include <string>
#include <vector>

struct Parser {
    int fun_depth = 0;
    std::vector<Token> tokens;
    // for multi stmts
    std::vector<Stmt *> statements;
    std::vector<std::string> errors;

    // for the single expression
    Expr *root;
    int curr_index = 0;

    // helper methods

    /// Get current Token
    Token peek();
    /// Get the consumed token
    Token previous();
    /// move the parser
    Token advance();
    /// check if it's last token
    bool is_at_end();
    /// Consume the type if present and advance
    Token consume(TokenType type, std::string message);

    bool match(TokenType type);
    bool check(TokenType type);

    Expr *expression();
    Expr *term();
    Expr *factor();
    Expr *primary();
    Expr *unary();
    Expr *comparison();
    Expr *equality();
    Expr *assignment();
    Expr *or_expr();
    Expr *and_expr();
    Expr *call();
    Expr *finish_call(Expr *calle, Token &name);

    std::vector<Stmt*> block();
    Stmt *expression_stmt();
    Stmt *statement();
    Stmt *prnt_stmt();
    Stmt *var_stmt();
    Stmt *block_stmt();
    Stmt *if_stmt();
    Stmt *while_stmt();
    Stmt *for_stmt();
    Stmt *function_stmt(std::string kind);
    Stmt *return_stmt();

    void uninitalize_var_error(Stmt *stmt);
    void synchronize();
  public:
    Parser(std::vector<Token> &tokens) {
        this->tokens = tokens;
    }
    std::vector<Stmt *> parse_stmt();

    Expr *parse() {
        return root = expression();
    }
    void visualize();
    void report_error();

    bool is_error() {
        return errors.size() > 0;
    }
};