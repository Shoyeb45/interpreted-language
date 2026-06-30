#include "node_types.hpp"

std::string node_type_to_string(NodeType type) {
    switch (type) {
    case NodeType::ASSIGN: {
        return "ASSIGN EXPR";
    }
    case NodeType::LOGICAL: {
        return "LOGICAL EXPR";
    }
    case NodeType::VARIABLE: {
        return "VARIABLE EXPR";
    }
    case NodeType::UNARY: {
        return "UNARY EXPR";
    }
    case NodeType::GROUP: {
        return "GROUP EXPR";
    }
    case NodeType::BINARY: {
        return "BINARY EXPR";
    }
    case NodeType::LITERAL: {
        return "LITERAL EXPR";
    }
    case NodeType::PRINT_STMT: {
        return "PRINT STMT";
    }
    case NodeType::VARIABLE_STMT: {
        return "VARIABLE STMT";
    }
    case NodeType::BLOCK_STMT: {
        return "BLOCK STMT";
    }
    case NodeType::IF_STMT: {
        return "IF STMT";
    }
    case NodeType::WHILE_STMT: {
        return "WHILE STMT";
    }
    case NodeType::EXPR_STMT: {
        return "EXPRESSION STMT";
    }
    case NodeType::FUNCTION_STMT: {
        return "FUNCTION STMT";
    }
    case NodeType::CALL: {
        return "CALL EXPR";
    }
    case NodeType::CLASS_STMT: {
        return "CLASS STMT";
    }
    case NodeType::GET: {
        return "GET EXPR";
    }
    case NodeType::SET: {
        return "SET EXPR";
    }
    }
    return "INVALID NODE TYPE";
}
