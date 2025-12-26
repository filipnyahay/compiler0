#include "include/scanner.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    lexer_t lexer;
    char* source = load_source_file(stdin);
    if (source == NULL)
        fprintf(stderr, "Error: unable to load source file\n");
    init_lexer(&lexer, source);

    while (1) {
        token_t* t = malloc(sizeof(token_t));
        token_init(t);
        scan_token(&lexer, t);
        if (t->type == TOKEN_EOF)
            break;
        print_token(t);
    }

    free(source);
    return 0;
}
