#ifndef _malloc_h
#define _malloc_h

#include <stddef.h>

void* malloc(size_t size);
void free(void* ptr);

void init_heap();
void print_heap();
void test_heap();

#endif
