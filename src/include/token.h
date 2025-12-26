#ifndef _TOKEN_H__
#define _TOKEN_H__

#include <stdlib.h>
#include "string.h"

#define TOKEN_FMT(t) (int)(t)->attr.len, (t)->attr.start

typedef enum {
    TOKEN_NO_TYPE,
    // tokens with value
    TOKEN_ID, TOKEN_INT, TOKEN_FLOAT, TOKEN_STRING,
    // keywords
    TOKEN_I64, TOKEN_F64, TOKEN_U8, TOKEN_NULL,
    TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_RETURN,
    TOKEN_VAR, TOKEN_FUN,
    // single character tokens
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_DIV,
    TOKEN_LESS, TOKEN_GREATER, TOKEN_ASSIGN, TOKEN_NOT,
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACKET, TOKEN_RBRACKET,
    TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_DOT, TOKEN_COMMA, TOKEN_SEMICOLUMN,
    // two character tokens
    TOKEN_LESS_EQ, TOKEN_GREATER_EQ,
    TOKEN_EQUAL, TOKEN_NOT_EQUAL,
    // error
    TOKEN_ERORR,
    // end of source code indicator
    TOKEN_EOF
} token_type;

typedef struct token {
    token_type type;
    string_t attr;
} token_t;

void token_init(token_t* token);
const char* type_to_string(token_type type);
void print_token(token_t* token);

#endif//_TOKEN_H__
