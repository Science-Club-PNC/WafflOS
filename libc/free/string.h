#ifndef __string_h__
#define __string_h__

#include <stddef.h>

size_t strlen(char* str);
char* string_reverse(char* target, size_t len);
char* int_to_string(int a, char* target, size_t len);

#endif
