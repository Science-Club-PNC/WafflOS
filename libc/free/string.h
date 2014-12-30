#ifndef __string_h__
#define __string_h__

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

size_t strlen(char* str);
char* string_reverse(char* target, size_t len);

char* long_to_dec_string(long a, char* target, size_t len);
char* ulong_to_dec_string(unsigned long a, char* target, size_t len);

inline char* short_to_dec_string(short a, char* target, size_t len) { return long_to_dec_string(a, target, len); }
inline char* int_to_dec_string(int a, char* target, size_t len) { return long_to_dec_string(a, target, len); }

inline char* ushort_to_dec_string(unsigned short a, char* target, size_t len) { return ulong_to_dec_string(a, target, len); }
inline char* uint_to_dec_string(unsigned int a, char* target, size_t len) { return ulong_to_dec_string(a, target, len); }

#if CHAR_MIN == 0  // if the type char is unsigned
    inline char* char_to_dec_string(char a, char* target, size_t len) { return ulong_to_dec_string(a, target, len); }
#endif

#endif
