#include <stdio.h>
#include "include/token.h"

void token_init(token_t* token) {
    token->type = TOKEN_NO_TYPE;
    str_empty_init(&token->attr);
}

const char* type_to_string(token_type type) {
    switch(type) {
        case TOKEN_NO_TYPE: return "NONE";
        case TOKEN_ERORR: return "ERROR";
        case TOKEN_ID: return "ID";
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_I64: return "I64";
        case TOKEN_F64: return "F64";
        case TOKEN_U8: return "U8";
        case TOKEN_NULL: return "NULL";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_VAR: return "VAR";
        case TOKEN_FUN: return "FUN";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULT: return "MULT";
        case TOKEN_DIV: return "DIV";
        case TOKEN_LESS: return "LESS";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_NOT: return "NOT";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_DOT: return "DOT";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLUMN: return "SEMICOLUMN";
        case TOKEN_LESS_EQ: return "LESS_EQ";
        case TOKEN_GREATER_EQ: return "GREATER_EQ";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_EOF: return "EOF";
        default: return "Unreachable";
    }
    return "Unreachable";
}

void print_token(token_t* token) {
    const char* type_str = type_to_string(token->type);
    if (token->attr.start!= NULL)
        printf("%s: %.*s\n", type_str, TOKEN_FMT(token));
    else
        printf("%s\n", type_str);
}
