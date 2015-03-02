#include <stddef.h>
#include <stdint.h>

#include "string.h"
#include "base.h"

char* memory_to_binary_string(const void* memory, size_t memory_size, char* buffer, size_t buffer_len)
{
    if (buffer_len < memory_size*8 + 1) return NULL;

    const uint8_t* cur_mem = memory + memory_size;
    const uint8_t* start_mem = memory;
    char* cur_buffer = buffer;

    while (cur_mem > start_mem) {
        cur_mem--;
        *cur_buffer = (*cur_mem & 0x80) ? '1' : '0';
        cur_buffer++;
        *cur_buffer = (*cur_mem & 0x40) ? '1' : '0';
        cur_buffer++;
        *cur_buffer = (*cur_mem & 0x20) ? '1' : '0';
        cur_buffer++;
        *cur_buffer = (*cur_mem & 0x10) ? '1' : '0';
        cur_buffer++;
        *cur_buffer = (*cur_mem & 0x08) ? '1' : '0';
        cur_buffer++;
        *cur_buffer = (*cur_mem & 0x04) ? '1' : '0';
        cur_buffer++;
        *cur_buffer = (*cur_mem & 0x02) ? '1' : '0';
        cur_buffer++;
        *cur_buffer = (*cur_mem & 0x01) ? '1' : '0';
        cur_buffer++;

    }

    *cur_buffer = '\0';

    return buffer;
}
