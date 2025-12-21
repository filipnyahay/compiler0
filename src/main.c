#include "scanner.h"
#include <stdio.h>

int main() {
    lexer_t lexer;
    char* source = load_source_file(stdin);
    if (source == NULL)
        fprintf(stderr, "Error: unable to load source file\n");
    init_lexer(&lexer, source);

    printf("%s", lexer.source_buffer);
    return 0;
}
