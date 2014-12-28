#include <stddef.h>

#include "string.h"

size_t strlen(char* str)
{
    char* pos = str;
    while (*pos) pos++;
    return pos - str;
}
