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

char* to_uppercase(char* str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 'a' + 'A';
        } else if (str[i] == '\0') {
            break;
        }
    }
    return str;
}

char* to_lowercase(char* str, size_t len) {
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

char* long_to_dec_string(long a, char* target, size_t len)
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

char* ulong_to_dec_string(unsigned long a, char* target, size_t len)
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

char* byte_to_hex_string(uint8_t a, char* target, size_t len)
{
    if (len < 3) return NULL;

    char cur_heximal = (a >> 4) & 0x0F;
    target[0] = (cur_heximal > 9) ? cur_heximal - 10 + 'a' : cur_heximal + '0';
    cur_heximal = a & 0x0F;
    target[1] = (cur_heximal > 9) ? cur_heximal - 10 + 'a' : cur_heximal + '0';
    target[2] = '\0';

    return target;
}

char* char_to_hex_string(unsigned char a, char* target, size_t len)
{
    if (len <= sizeof(a)*2) return NULL;

    const uint8_t* a_ptr = (const uint8_t*)&a + sizeof(a);

    for (size_t i = 0; i < sizeof(a); i++) {
        byte_to_hex_string(*(a_ptr - i - 1), target + i*2, 3);
    }

    return target;
}

char* short_to_hex_string(unsigned short a, char* target, size_t len)
{
    if (len <= sizeof(a)*2) return NULL;

    const uint8_t* a_ptr = (const uint8_t*)&a + sizeof(a);

    for (size_t i = 0; i < sizeof(a); i++) {
        byte_to_hex_string(*(a_ptr - i - 1), target + i*2, 3);
    }

    return target;
}

char* int_to_hex_string(unsigned int a, char* target, size_t len)
{
    if (len <= sizeof(a)*2) return NULL;

    const uint8_t* a_ptr = (const uint8_t*)&a + sizeof(a);

    for (size_t i = 0; i < sizeof(a); i++) {
        byte_to_hex_string(*(a_ptr - i - 1), target + i*2, 3);
    }

    return target;
}

char* long_to_hex_string(unsigned long a, char* target, size_t len)
{
    if (len <= sizeof(a)*2) return NULL;

    const uint8_t* a_ptr = (const uint8_t*)&a + sizeof(a);

    for (size_t i = 0; i < sizeof(a); i++) {
        byte_to_hex_string(*(a_ptr - i - 1), target + i*2, 3);
    }

    return target;
}

int dec_string_to_int(const char* source, size_t len) {
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

