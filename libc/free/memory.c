#include <stddef.h>
#include <stdint.h>

#include "memory.h"

//TODO: write word alignment optimalization
void* memmove(void* dst_ptr, const void* src_ptr, size_t size)
{
    uint8_t* dst;
    const uint8_t* src;

    if (dst_ptr < src_ptr) {
        const uint8_t* src_end = (const uint8_t*)src_ptr + size;

        dst = (uint8_t*)dst_ptr;
        src = (const uint8_t*)src_ptr;

        while (src < src_end) {
            *dst = *src;
            src++;
            dst++;
        }
    } else {
        const uint8_t* src_start = (const uint8_t*)src_ptr;

        dst = (uint8_t*)dst_ptr + size - 1;
        src = (const uint8_t*)src_ptr + size - 1;

        while (src >= src_start) {
            *dst = *src;
            src--;
            dst--;
        }
    }

    return dst_ptr;
}

//TODO: write word alignment optimalization
void* memcpy(void* dst_ptr, const void* src_ptr, size_t size)
{
    uint8_t* dst = (uint8_t*)dst_ptr;
    const uint8_t* src = (const uint8_t*)src_ptr;

    const uint8_t* src_end = (const uint8_t*)src_ptr + size;

    while (src < src_end) {
        *dst = *src;
        src++;
        dst++;
    }

    return dst_ptr;
}

void* memset(void* dst_ptr, int src, size_t size)
{
    const uint8_t source = (const uint8_t)src;

    uint8_t* dst = (uint8_t*)dst_ptr;
    const uint8_t* dst_end = dst_ptr + size;

    while (dst < dst_end) {
        *dst = source;
        dst++;
    }

    return dst_ptr;
}
