#include <stddef.h>
#include <stdint.h>

#include "string.h"
#include "base.h"

char* memory_to_hex_string(const void* memory, size_t memory_size, char* buffer, size_t buffer_len)
{
    if (buffer_len < memory_size*2 + 1) return NULL;

    const uint8_t* cur_mem = memory + memory_size;
    const uint8_t* start_mem = memory;
    char* cur_buffer = buffer;
    char cur_heximal;

    while (cur_mem > start_mem) {
        cur_mem--;

        cur_heximal = (*cur_mem >> 4) & 0x0F;
        *cur_buffer = (cur_heximal > 9) ? cur_heximal - 10 + 'a' : cur_heximal + '0';
        cur_buffer++;

        cur_heximal = *cur_mem & 0x0F;
        *cur_buffer = (cur_heximal > 9) ? cur_heximal - 10 + 'a' : cur_heximal + '0';
        cur_buffer++;
    }

    *cur_buffer = '\0';

    return buffer;
}
