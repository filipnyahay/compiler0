#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "include/scanner.h"
#include "include/token.h"
#include "include/string.h"

void print_until_newline(const char* start) {
    const char* tmp = start;
    while (*tmp != '\n' && *tmp != '\0') {
        putc(*tmp, stderr);
        tmp++;
    }
    putc('\n', stderr);
}

void print_lexical_error(const char* msg, lexer_t* lexer, token_t* token) {
    fprintf(stderr, "%d:%d ", lexer->line, (int)(token->attr.start - lexer->current_line_start));
    fprintf(stderr, "Error: %s\n", msg);
    fprintf(stderr, "\t| ");
    print_until_newline(lexer->current_line_start);
    fprintf(stderr, "\t| ");
    for (int i = 0; i < (int)(token->attr.start - lexer->current_line_start); i++)
        putc(' ', stderr);
    fprintf(stderr, "^\n");
}

char* load_source_file(FILE *fin) {
    assert(fin != NULL);

    if (fseek(fin, 0L, SEEK_END) != 0) return NULL;

    long buffer_len = ftell(fin);
    if (buffer_len == -1L) return NULL; 

    rewind(fin);

    char* input_buffer = malloc(buffer_len+1);
    if (input_buffer == NULL) return NULL; 

    fread(input_buffer, sizeof(char), buffer_len, fin);
    if (ferror(fin) != 0) return NULL;

    input_buffer[buffer_len] = '\0';

    return input_buffer;
}

void init_lexer(lexer_t *lexer, char* src_file) {
    assert(lexer != NULL);
    assert(src_file != NULL);

    lexer->src_start= src_file;
    lexer->src_current = src_file;
    lexer->current_line_start = src_file;
}

bool is_at_end(lexer_t* lexer) {
    return *lexer->src_current == '\0';
}

char peek(lexer_t* lexer) {
    return *lexer->src_current;
}

char peek_next(lexer_t* lexer) {
    if (is_at_end(lexer))
        return '\0';
    return *(lexer->src_current+1);
}

void consume(lexer_t* lexer) {
    lexer->src_current++;
}

bool match(lexer_t* lexer, char to_match) {
    return *lexer->src_current == to_match;
}

bool match_next(lexer_t* lexer, char to_match) {
    return peek_next(lexer) == to_match;
}

char* current_pos(lexer_t* lexer) {
    return lexer->src_current;
}

void skip_spaces(lexer_t* lexer) {
    while (1) {
        char curr_char = peek(lexer);
        switch(curr_char) {
            case '\n':
                ++lexer->line;
                lexer->current_line_start = lexer->src_current+1;
            case ' ':
            case '\t':
            case '\r':
                consume(lexer);
                break;
            default: return;
        }
    }
}

int is_single_char(lexer_t *lexer, char c, token_type* type) {
    switch (c) {
        case '+': *type = TOKEN_PLUS;       consume(lexer); return 0;
        case '-': *type = TOKEN_MINUS;      consume(lexer); return 0;
        case '*': *type = TOKEN_MULT;       consume(lexer); return 0;
        case '/': *type = TOKEN_DIV;        consume(lexer); return 0;
        case '(': *type = TOKEN_LPAREN;     consume(lexer); return 0;
        case ')': *type = TOKEN_RPAREN;     consume(lexer); return 0;
        case '[': *type = TOKEN_LBRACKET;   consume(lexer); return 0;
        case ']': *type = TOKEN_RBRACKET;   consume(lexer); return 0;
        case '{': *type = TOKEN_LBRACE;     consume(lexer); return 0;
        case '}': *type = TOKEN_RBRACE;     consume(lexer); return 0;
        case '.': *type = TOKEN_DOT;        consume(lexer); return 0;
        case ',': *type = TOKEN_COMMA;      consume(lexer); return 0;
        case ';': *type = TOKEN_SEMICOLUMN; consume(lexer); return 0;
        default: return 1;
    }
    fprintf(stderr, "Error: unreachable code\n");
    return 1;
}

int is_two_char(lexer_t *lexer, char c, token_type* type) {
    switch (c) {
        case '<':
            // *type = peek -> match(lexer_t lexer, char to_match);
            *type = match_next(lexer, '=') ? TOKEN_LESS_EQ : TOKEN_LESS;
            consume(lexer);
            return 0;
        case '>':
            *type = match_next(lexer, '=') ? TOKEN_GREATER_EQ : TOKEN_GREATER;
            consume(lexer);
            return 0;
        case '=':
            *type = match_next(lexer, '=') ? TOKEN_EQUAL : TOKEN_ASSIGN;
            consume(lexer);
            return 0;
        case '!':
            *type = match_next(lexer, '=') ? TOKEN_NOT_EQUAL : TOKEN_NOT;
            consume(lexer);
            return 0;
        default: return 1;
    }
    fprintf(stderr, "Error: unreachable code\n");
    return 1;
}

static const char* kwds[] = {
    [TOKEN_I64] =    "i64",
    [TOKEN_F64] =    "f64",
    [TOKEN_U8] =     "u8",
    [TOKEN_IF] =     "if",
    [TOKEN_ELSE] =   "else",
    [TOKEN_NULL] =   "null",
    [TOKEN_RETURN] = "return",
    [TOKEN_VAR] =    "var",
    [TOKEN_FUN] =    "fun",
    [TOKEN_WHILE] =  "while",
};
const size_t kwds_len = 15;

int scan_identifier(lexer_t* lexer, token_t* token) {
    token->attr.start = current_pos(lexer);
    size_t id_len = 1;
    char next_char;
    while (!is_at_end(lexer)) {
        next_char = peek_next(lexer);
        if (isalnum(next_char) || next_char == '_') {
            ++id_len;
            consume(lexer);
        }
        else { break; }
    }

    consume(lexer);
    token->attr.len = id_len;
    token->attr.cap = id_len*2;
    token->type = TOKEN_ID;

    for (size_t i = 0; i < kwds_len; i++) {
        if (kwds[i] == NULL) { continue; }
        if (str_cchar_cmp(&token->attr, kwds[i]) == 0) {
            str_empty_init(&token->attr);
            token->type = i;
            break;
        }
    }

    return SUCCESS;
}

int scan_string(lexer_t* lexer, token_type* type) {
    size_t str_len = 1;
    char next_char;
    while(!is_at_end(lexer) || match_next(lexer, '"')) {
        // TODO: put match next inside body, outside loop -> error string not ended
        next_char = peek_next(lexer);
        if (isascii(next_char)) {
            ++str_len;
            consume(lexer);
        }
        else { break; }
    }
    consume(lexer);
    *type = TOKEN_STRING;
    return str_len;
}

bool check_overflow(string_t str, bool is_float) {
    if (is_float) {
        double res = strtod(str.start, &str.start+str.len);
        if (STRTOD_BOUNDS(res))
            return true;
    }
    else {
        long long res = strtoll(str.start, &str.start+str.len, 10);
        if (STRTOLL_BOUNDS(res))
            return true;
    }
    return false;
}

int scan_number(lexer_t* lexer, token_t* token) {
    token->attr.start = current_pos(lexer);
    size_t num_len = 1;
    char next_char;
    bool is_float = false;
    while(!is_at_end(lexer)) {
        next_char = peek_next(lexer);
        if (isdigit(next_char)) {
            ++num_len;
            consume(lexer);
        }
        else if (next_char == '.' && is_float == false) {
            ++num_len;
            consume(lexer);
            is_float = true;
            if (!isdigit(peek_next(lexer))) {
                token->type = TOKEN_ERORR;
                return INVALID_CHAR;
            }
        }
        else if (isalpha(next_char)) {
            token->type = TOKEN_ERORR;
            return INVALID_CHAR;
        }
        else {
            break;
        }
    }

    consume(lexer);
    token->attr.len = num_len;
    token->attr.cap = num_len*2;

    bool overflow = check_overflow(token->attr, is_float);
    if (overflow) {
        print_lexical_error("numeric literal out of bounds", lexer, token);
        exit(1);
    }

    if (is_float) token->type = TOKEN_FLOAT;
    else          token->type = TOKEN_INT;
    return SUCCESS;
}

int scan_token(lexer_t* lexer, token_t *token) {
    assert(lexer != NULL);
    assert(token != NULL);

    skip_spaces(lexer);

    char curr_char = peek(lexer);
    token_type new_type;

    if (is_at_end(lexer)) {
        token->type = TOKEN_EOF;
        return 0;
    }
    else if (is_single_char(lexer, curr_char, &new_type) == 0) {
        token->type = new_type;
        return 0;
    }
    else if (is_two_char(lexer, curr_char, &new_type) == 0) {
        token->type = new_type;
        return 0;
    }
    else if (isalpha(curr_char)) {
        int status = scan_identifier(lexer, token);
        if (status == SUCCESS)
            return 0;
        else
            fprintf(stderr, "Error: unreachable code\n");
    }
    else if (curr_char == '"') {
        token->attr.start = current_pos(lexer);
        size_t str_len = scan_string(lexer, &new_type);

        token->attr.len = str_len;
        token->attr.cap = str_len*2;
        token->type = new_type;
        return 0;
    }
    else if (isdigit(curr_char)) {
        int status = scan_number(lexer, token);
        if (status != SUCCESS)
            exit(1);
        return 0;
    }
    print_lexical_error("illegal character", lexer, token);
    exit(1);
    return 1;
}
