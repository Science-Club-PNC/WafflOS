#ifndef __memory_h__
#define __memory_h__

#include <stddef.h>

void* memmove(void* dst_ptr, const void* src_ptr, size_t size);
void* memcpy(void* dst_ptr, const void* src_ptr, size_t size);
void* memset(void* dst_ptr, int src, size_t size);

#endif
