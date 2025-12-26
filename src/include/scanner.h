#ifndef _SCANNER_H__
#define _SCANNER_H__

#include <stdio.h>
#include <stdbool.h>
#include "token.h"

#define STRTOLL_BOUNDS(n) \
    (n) == LONG_MAX || (n) == LLONG_MAX || \
    (n) == LONG_MIN || (n) == LLONG_MIN

#define STRTOD_BOUNDS(n) \
    (n) == HUGE_VAL || (n) == -HUGE_VAL

typedef enum {
    SUCCESS,
    INVALID_CHAR,
    TOO_LONG
} errors;

typedef struct lexer {
    char* src_start;
    char* src_current;
    char* current_line_start;
    unsigned line;
} lexer_t;


void print_until_newline(const char* start);

char* load_source_file(FILE *fin);
void init_lexer(lexer_t *lexer, char* src_file);

bool is_at_end(lexer_t* lexer);
char peek(lexer_t* lexer);
char peek_next(lexer_t* lexer);
void consume(lexer_t* lexer);
bool match(lexer_t* lexer, char to_match);
bool match_next(lexer_t* lexer, char to_match);
char* current_pos(lexer_t* lexer);

void skip_spaces(lexer_t* lexer);
int is_single_char(lexer_t *lexer, char c, token_type* type);
int is_two_char(lexer_t *lexer, char c, token_type* type);
int scan_identifier(lexer_t* lexer, token_t* token);
int scan_string(lexer_t* lexer, token_type* type);
int scan_number(lexer_t* lexer, token_t* token);

int scan_token(lexer_t* lexer, token_t *token);

#endif//_SCANNER_H__
