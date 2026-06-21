#include "executor.hpp"
#include "../parser/stmt.hpp"

std::string Executor::executeExprStmt(ExprStmt *stmt) {
    evaluator->set_root(stmt->expr);
    return evaluator->evaluate();
}

std::string Executor::executePrntStmt(PrintStmt *prntStmt) {
    evaluator->set_root(prntStmt->expr);
    return evaluator->evaluate();
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
    // error
}

void Executor::execute() {
    for (Stmt* stmt : statements) {
        std::cout << executeStmt(stmt) << "\n";
    }
}