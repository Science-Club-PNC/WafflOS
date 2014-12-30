#ifndef __string_h__
#define __string_h__

#include <stddef.h>
#include <bool.h>

size_t strlen(char* str);
char* string_reverse(char* target, size_t len);

char* to_dec_string(void b, char* target, size_t len, bool sign);
#define char_to_dec_string(a, target, len) to_dec_string((void)a, target, len, false)
#define short_to_dec_string(a, target, len) to_dec_string((void)a, target, len, true)
#define int_to_dec_string(a, target, len) to_dec_string((void)a, target, len, true)
#define long_to_dec_string(a, target, len) to_dec_string((void)a, target, len, true)
#define ushort_to_dec_string(a, target, len) to_dec_string((void)a, target, len, false)
#define uint_to_dec_string(a, target, len) to_dec_string((void)a, target, len, false)
#define ulong_to_dec_string(a, target, len) to_dec_string((void)a, target, len, false)

#endif
