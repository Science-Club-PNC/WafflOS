#ifndef __string_h__
#define __string_h__

#include <stddef.h>

size_t strlen(char* str);
char* string_reverse(char* target, size_t len);

char* char_to_dec_string(unsigned char a, char* target, size_t len);

char* short_to_dec_string(short a, char* target, size_t len);
char* int_to_dec_string(int a, char* target, size_t len);
char* long_to_dec_string(long a, char* target, size_t len);

char* ushort_to_dec_string(unsigned short a, char* target, size_t len);
char* uint_to_dec_string(unsigned int a, char* target, size_t len);
char* ulong_to_dec_string(unsigned long a, char* target, size_t len);

#endif
