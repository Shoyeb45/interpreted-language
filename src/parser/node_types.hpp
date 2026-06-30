#pragma once

#include <string>

enum NodeType {
    LITERAL,
    BINARY,
    GROUP,
    UNARY,
    VARIABLE,
    LOGICAL,
    ASSIGN,
    CALL,
    GET, // getter for the instances
    SET, // setter for the instances
    PRINT_STMT,
    EXPR_STMT,
    VARIABLE_STMT,
    BLOCK_STMT,
    IF_STMT,
    WHILE_STMT,
    FUNCTION_STMT,
    RETURN_STMT,
    CLASS_STMT,
};

std::string node_type_to_string(NodeType type);