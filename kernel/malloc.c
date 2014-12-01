#include "terminal.h"
#include "malloc.h"
#include <string.h>
 
//#define buffer_size 0x80000

#define buffer_size 0x40
uint8_t buffer[buffer_size];

struct mem_tag* first_mem_tag;
struct mem_tag* last_mem_tag;

void init_mem()
{	
	first_mem_tag = (struct mem_tag*)(buffer + buffer_size) - 1;
	last_mem_tag = first_mem_tag;
	
	first_mem_tag->next = NULL;
	first_mem_tag->start = buffer;
	first_mem_tag->end = first_mem_tag;
}

void print_mem()
{	
	struct mem_tag* cur_tag = first_mem_tag;	

	for (size_t i = 0; i < buffer_size; i++) {
		if (cur_tag == NULL)
		{
			terminal_setcolor(color_white, color_red);
		} else if (cur_tag->start <= (void*)(buffer+i)) {
			if (cur_tag->end > (void*)(buffer+i)) {
				terminal_setcolor(color_white, color_green);
			} else {
				cur_tag = cur_tag->next;
				terminal_setcolor(color_white, color_red);
			}
		} else {
			terminal_setcolor(color_white, color_red);
		}
		

		uint8_t cur_heximal = ((buffer[i] >> 4) & 0x0F); 
		terminal_writechar((cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0');
		cur_heximal = (buffer[i] & 0x0F);
		terminal_writechar((cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0');
		if (i % 32 == 31) {
			terminal_setcolor(color_white, color_black);			
			terminal_writechar('\n');
		} else if (i % 4 == 3) {
			terminal_setcolor(color_white, color_black);		
			terminal_writechar(' ');
		}
	}
	terminal_setcolor(color_white, color_black);
}

void test_mem()
{	
	print_mem();
	terminal_writechar('\n');	

	uint32_t* a = malloc(4);
	*a = 0xAAAAAAAA;
	uint32_t* b = malloc(4);
	*b = 0xBBBBBBBB;
	uint32_t* c = malloc(4);
	*c = 0xCCCCCCCC;

	print_mem();
	terminal_writechar('\n');	

	free(a);
	free(b);

	print_mem();
	terminal_writechar('\n');
	
	uint16_t* d = malloc(2);
	*d = 0xDDDD;

	print_mem();
	terminal_writechar('\n');
}

inline void add_mem_tag(void* start, void* end, struct mem_tag* tag_before)
{	
	struct mem_tag* tag_spot = (struct mem_tag*)(buffer + buffer_size) - 1;
	while (tag_spot->end != NULL)
	{
		tag_spot--;
	}

	tag_spot->start = start;
	tag_spot->end = end;
	if (tag_before == NULL) {
		tag_spot->next = first_mem_tag;
		first_mem_tag = tag_spot;		
	} else {
		tag_spot->next = tag_before->next;
		tag_before->next = tag_spot;
	}
}

inline void remove_mem_tag(struct mem_tag* mem_tag, struct mem_tag* tag_before)
{
	if (tag_before == NULL) {
		first_mem_tag = mem_tag->next;
	} else {
		tag_before->next = mem_tag->next;
	}

	mem_tag->end = NULL;
}

void* malloc(size_t requested_size)
{
	void* ptr = NULL;
	size_t actual_size = requested_size + sizeof(size_t);

	struct mem_tag* cur_mem_tag = first_mem_tag;
	struct mem_tag* last_mem_tag = NULL;

	while (cur_mem_tag != NULL) {
		size_t mem_size = cur_mem_tag->end - cur_mem_tag->start;
		if (mem_size >= actual_size) {
			ptr = cur_mem_tag->start + sizeof(size_t);
			*(size_t*)(cur_mem_tag->start) = requested_size;
			
			if (mem_size > actual_size) {
				cur_mem_tag->start += actual_size;
				terminal_writestring("log: shrink start\n");
			} else {
				remove_mem_tag(cur_mem_tag, last_mem_tag);
				terminal_writestring("log: remove tag\n");
			}

			break;
		}

		last_mem_tag = cur_mem_tag;
		cur_mem_tag = cur_mem_tag->next;
	}

	return ptr;
}

void free(void* ptr)
{
	struct mem_tag* before_tag = NULL;
	struct mem_tag* after_tag = first_mem_tag;

	void* mem_start = ptr - sizeof(size_t);
	size_t mem_size = *(size_t*)mem_start + sizeof(size_t);
	void* mem_end = mem_start + mem_size;

	if (mem_start < (void*)buffer) {
		terminal_writestring("error: mem1\n");
		return;
	}

	while(after_tag != NULL) {
		if (mem_end < after_tag->start) {
			if (before_tag == NULL || mem_start >= before_tag->end) {
				if (mem_end == after_tag->start) {
					after_tag->start = mem_start;
					terminal_writestring("log: grow start\n");
				} else if (before_tag != NULL && mem_start == before_tag->end) {
					before_tag->end = mem_end;
					terminal_writestring("log: grow end\n");
				} else {
					add_mem_tag(mem_start, mem_end, before_tag);
					terminal_writestring("log: add tag\n");
				}
				return;
			} else {
				terminal_writestring("error: mem2\n");
				return;
			}
		}			
				
		before_tag = after_tag;
		after_tag = after_tag->next;
	}
	terminal_writestring("error: mem3\n");
	return;
}
