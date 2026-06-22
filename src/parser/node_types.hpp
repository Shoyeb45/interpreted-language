#pragma once

enum NodeType { 
    LITERAL, 
    BINARY, 
    GROUP, 
    UNARY, 
    VARIABLE,
    PRINT_STMT, 
    EXPR_STMT,
    VARIABLE_STMT,
    ASSIGN,
};
