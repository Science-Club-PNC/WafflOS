#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "string.h"

size_t strlen(char* str)
{
    char* pos = str;
    while (*pos) pos++;
    return pos - str;
}

char* string_reverse(char* target, size_t len)
{
    char* i = target;
    char* j = target + len - 1;

    while (i < j) {
        char temp = *i;
        *i = *j;
        *j = temp;
        i++;
        j--;
    }

    return target;
}

//TODO: Don't depend on len
#define INT_TO_DEC_STRING_CODE {            \
    bool negative_input = false;            \
    if (a<0) {                              \
        a = -a;                             \
        negative_input = true;              \
    }                                       \
                                            \
    size_t i;                               \
    for (i = 0; i < len - 1; i++) {         \
        int extract = a % 10;               \
        target[i] = (char)extract + '0';    \
        a = (a - extract) / 10;             \
        if (a == 0) {                       \
            i++;                            \
            break;                          \
        }                                   \
    }                                       \
    if (negative_input && (i < len - 1)) {  \
        target[i] = '-';                    \
        i++;                                \
    }                                       \
    target[i] = '\0';                       \
                                            \
    return string_reverse(target, i);       \
}

#define UINT_TO_DEC_STRING_CODE {           \
    size_t i;                               \
    for (i = 0; i < len - 1; i++) {         \
        int extract = a % 10;               \
        target[i] = (char)extract + '0';    \
        a = (a - extract) / 10;             \
        if (a == 0) {                       \
            i++;                            \
            break;                          \
        }                                   \
    }                                       \
    target[i] = '\0';                       \
                                            \
    return string_reverse(target, i);       \
}

// Code assumes that char is unsigned
char* char_to_dec_string(unsigned char a, char* target, size_t len)
UINT_TO_DEC_STRING_CODE

char* short_to_dec_string(short a, char* target, size_t len)
INT_TO_DEC_STRING_CODE
char* int_to_dec_string(int a, char* target, size_t len)
INT_TO_DEC_STRING_CODE
char* long_to_dec_string(long a, char* target, size_t len)
INT_TO_DEC_STRING_CODE

char* ushort_to_dec_string(unsigned short a, char* target, size_t len)
UINT_TO_DEC_STRING_CODE
char* uint_to_dec_string(unsigned int a, char* target, size_t len)
UINT_TO_DEC_STRING_CODE
char* ulong_to_dec_string(unsigned long a, char* target, size_t len)
UINT_TO_DEC_STRING_CODE

