#ifndef _malloc_h
#define _malloc_h

#include <stddef.h>
#include <stdint.h>

struct mem_tag
{
	void* start;
	void* end;
	struct mem_tag* next;	
};

void* malloc(size_t size);
void free(void* ptr);

void init_mem();
void print_mem();
void test_mem();

#endif
