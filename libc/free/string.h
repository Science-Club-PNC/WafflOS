#ifndef __string_h__
#define __string_h__

#include <stddef.h>

char *string_reverse(char *target, size_t len);
char *int_to_string(int a, char *target, size_t len);
size_t strlen(char *str);

#endif
