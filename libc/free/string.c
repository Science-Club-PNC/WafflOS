#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "string.h"
#include "base.h"

size_t strlen(char* str)
{
    char* pos = str;
    while (*pos) pos++;
    return pos - str;
}

char* to_uppercase(char* str, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 'a' + 'A';
        } else if (str[i] == '\0') {
            break;
        }
    }
    return str;
}

char* to_lowercase(char* str, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] - 'A' + 'a';
        } else if (str[i] == '\0') {
            break;
        }
    }
    return str;
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
