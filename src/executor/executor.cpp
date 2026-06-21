#include "executor.hpp"
#include "../parser/stmt.hpp"

std::string Executor::executeExprStmt(ExprStmt *stmt) {
    evaluator->set_root(stmt->expr);
    std::string result = evaluator->evaluate();
    if (evaluator->errors.size() > 0) {
        evaluator->report_error();
        std::exit(70);
    }
    return "";
}

std::string Executor::executePrntStmt(PrintStmt *prntStmt) {
    evaluator->set_root(prntStmt->expr);
    std::string result = evaluator->evaluate();
    if (evaluator->errors.size() > 0) {
        evaluator->report_error();
        std::exit(70);
    }  
    return result;
}

std::string Executor::executeStmt(Stmt *stmt) {
    switch (stmt->type) {
    case NodeType::EXPR_STMT: {
        return executeExprStmt(static_cast<ExprStmt *>(stmt));
    }
    case NodeType::PRINT_STMT: {
        return executePrntStmt(static_cast<PrintStmt *>(stmt));
    }
    }
    return "";
}

void Executor::execute() {
    std::string output = "";
    for (Stmt* stmt : statements) {
        output = executeStmt(stmt);
        std::cout << (output != "" ? output + "\n": "");
    }
}