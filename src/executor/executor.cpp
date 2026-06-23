#include "executor.hpp"
#include "../parser/stmt.hpp"

void Executor::execute_expr_stmt(ExprStmt *stmt) {
    evaluator->set_root(stmt->expr);
    std::string result = evaluator->evaluate();

    if (evaluator->errors.size() > 0) {
        evaluator->report_error();
        std::exit(70);
    }
}

void Executor::execute_var_stmt(VariableStmt *varStmt) {
    RuntimeValue value = evaluator->evaluate(varStmt->expr);
    if (evaluator->errors.size() > 0) {
        evaluator->report_error();
        std::exit(70);
    }
    evaluator->global->set(varStmt->identifier.lexeme, value);
}

void Executor::execute_prnt_stmt(PrintStmt *prntStmt) {
    evaluator->set_root(prntStmt->expr);
    std::string result = evaluator->evaluate();

    if (evaluator->errors.size() > 0) {
        evaluator->report_error();
        std::exit(70);
    }

    std::cout << result << "\n";
}

void Executor::execute_block_stmt(BlockStmt *block_stmt, EnvironmentTable *environment) {
    EnvironmentTable *prev = this->evaluator->global;
    
    // change the environment
    this->evaluator->global = environment;
    
    // execute the statements
    for (int i = 0; i < block_stmt->stmts.size(); i++) {
        if (block_stmt->stmts[i]) execute_stmt(block_stmt->stmts[i]);
    }
    
    // change it into global one
    this->evaluator->global = prev;
}

void Executor::execute_stmt(Stmt *stmt) {
    switch (stmt->type) {
    case NodeType::EXPR_STMT: {
        execute_expr_stmt(static_cast<ExprStmt *>(stmt));
        break;
    }
    case NodeType::PRINT_STMT: {
        execute_prnt_stmt(static_cast<PrintStmt *>(stmt));
        break;
    }
    case NodeType::VARIABLE_STMT: {
        execute_var_stmt(static_cast<VariableStmt *>(stmt));
        break;
    }
    case NodeType::BLOCK_STMT: {
        execute_block_stmt(static_cast<BlockStmt *>(stmt), new EnvironmentTable(this->evaluator->global));
        break;
    }
    }
}

void Executor::execute() {
    std::string output = "";

    for (Stmt *stmt : statements) {
        execute_stmt(stmt);
    }
}