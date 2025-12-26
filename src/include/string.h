#ifndef _STRING_H__
#define _STRING_H__

#include <string.h>

typedef struct string {
    char* start;
    size_t len;
    size_t cap;
} string_t;

string_t* cchar2str(const char* str);
string_t* str_create(char* start, size_t len);
void str_empty_init(string_t* str);
int str_cchar_cmp(string_t* str1, const char* str2);

#endif//_STRING_H__
