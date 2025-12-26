#include "include/string.h"
#include <string.h>

#include <stdio.h>

void str_empty_init(string_t* str) {
    str->start = NULL;
    str->len = 0;
    str->cap = 0;
}

int str_cchar_cmp(string_t* str1, const char* str2) {
    size_t cchar_len = strlen(str2);
    if (str1->len > cchar_len) {
        return 1;
    }
    else if (str1->len < cchar_len) {
        return -1;
    }
    for (size_t i = 0; i < str1->len; i++) {
        if (str1->start[i] != str2[i]) {
            return -1;
        }
    }
    return 0;
}
