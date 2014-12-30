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
char* to_dec_string(void b, char* target, size_t len, bool sign)
{
    if (sign) {
        long a = b;
    } else {
        unsigned long a = b;
    }

    bool negative_input = false;
    if (a < 0 && sign) {
        a = -a;
        negative_input = true;
    }

    size_t i;
    for (i = 0; i < len - 1; i++) {
        int extract = a % 10;
        target[i] = (char)extract + '0';
        a = (a - extract) / 10;
        if (a == 0) {
            i++;
            break;
        }
    }
    if (negative_input && (i < len - 1)) {
        target[i] = '-';
        i++;
    }
    target[i] = '\0';

    return string_reverse(target, i);
}
