#pragma once

enum NodeType { 
    LITERAL, 
    BINARY, 
    GROUP, 
    UNARY, 
    VARIABLE,
    ASSIGN,
    PRINT_STMT, 
    EXPR_STMT,
    VARIABLE_STMT,
    BLOCK_STMT,
    IF_STMT,
};
