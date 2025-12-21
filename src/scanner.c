#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "scanner.h"

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

    lexer->source_buffer = src_file;
    lexer->current_line_start = src_file;
}
