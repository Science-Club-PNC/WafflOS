#include <stddef.h>
#include <stdbool.h>

#include "string.h"
#include "base.h"

char* number_to_dec_string(long a, char* target, size_t len)
{
    bool negative_input = false;
    if (a<0) {
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

char* unsigned_number_to_dec_string(unsigned long a, char* target, size_t len)
{
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
    target[i] = '\0';

    return string_reverse(target, i);
}

int dec_string_to_int(const char* source, size_t len)
{
    if (len == 0) return 0;

    bool output_is_negative = false;
    int a = 0;
    size_t i = 0;

    if (source[0] == '-') {
        output_is_negative = true;
        i++;
    } else if (source[0] == '+') {
        i++;
    }
    while (i<len) {
        char c = source[i];
        if (c >= '0' && c <= '9') {
            a *= 10;
            a += c - '0';
        } else {
            break;
        }
        i++;
    }

    if (output_is_negative) a = -a;

    return a;
}
