#include <stddef.h>

#include "memory.h"

//TODO: write word alignment optimalization
void* memmove(void* dst_ptr, const void* src_ptr, size_t size) 
{
    unsigned char* dst;
    const unsigned char* src;

    if (dst_ptr < src_ptr) {
        const unsigned char* src_end = (const unsigned char*)src_ptr + size;

        dst = (unsigned char*)dst_ptr;
        src = (const unsigned char*)src_ptr;

        while (src < src_end) {
            *dst = *src;
            src++;
            dst++;
        }
    } else {
        const unsigned char* src_start = (const unsigned char*)src_ptr;

        dst = (unsigned char*)dst_ptr + size - 1;
        src = (const unsigned char*)src_ptr + size - 1;

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
    unsigned char* dst = (unsigned char*)dst_ptr;
    const unsigned char* src = (const unsigned char*)src_ptr;

    const unsigned char* src_end = (const unsigned char*)src_ptr + size;

    while (src < src_end) {
        *dst = *src;
        src++;
        dst++;
    }

    return dst_ptr;
}

void* memset(void* dst_ptr, int src, size_t size)
{
    const unsigned char source = (const unsigned char)src;
    
    unsigned char* dst = (unsigned char*)dst_ptr;
    const unsigned char* dst_end = dst_ptr + size;

    while (dst < dst_end) {
        *dst = source;
        dst++;
    }
    
    return dst_ptr;
}
