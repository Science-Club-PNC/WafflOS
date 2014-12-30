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
    target[0] = (cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0';
    cur_heximal = a & 0x0F;
    target[1] = (cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0';
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
