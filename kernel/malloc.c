#include "terminal.h"
#include "malloc.h"

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Define struct to point towards the end and start of used memory
struct mem_tag{
    void* start;
    void* end;
    struct mem_tag* next;
};

// Add an array to the stack to act as heap.
// TODO: Create the heap dynamicly relative to the available size.
#define heap_size 0x40
uint8_t heap_array[heap_size];

// Define pointers to the start and endof the heap (this area will shrink when adding memory tags). Note that heap_end is just outside of the heap.
void* heap_start;
void* heap_end;

// Define the memory in the heap used for tagging memory.
struct mem_tag* tag_space_start;
struct mem_tag* tag_space_end;

// Define pointers to point towards the first and last memory tag
struct mem_tag* first_tag;
struct mem_tag* last_tag;

// Define the tag that has the lowest location in memory.
struct mem_tag* lowest_tag;

// Define the size of the buffer (in mem_tags) that reserves some extra memory after the last mem_tag to act as buffer when memory is (nearly) full. Should be atleast 1 to append extra tags.
size_t tag_space_buffer_size;

// Initializes the heap (may be used to completely reset the heap)
void init_heap()
{
    tag_space_buffer_size = 1;

    tag_space_end = (struct mem_tag*)(heap_array + heap_size);
    tag_space_start = tag_space_end - tag_space_buffer_size;

    heap_start = (void*)heap_array;
    heap_end = (void*)tag_space_start;

    first_tag = NULL;
    last_tag = NULL;
    lowest_tag = NULL;
}

void resize_tag_space()
{
    // Resize the tag space to have tag space buffer available.
    if (lowest_tag == NULL) {
        tag_space_start = tag_space_end - tag_space_buffer_size;
    } else {
        tag_space_start = lowest_tag - tag_space_buffer_size;
    }
    // Prevent that the tag space leaks into used heap space.
    if ((last_tag != NULL) && (last_tag->end > (void*)tag_space_start)) {
        terminal_writestring("log: space leak\n");
        tag_space_start = last_tag->end;
    }
    // Resize the heap to correspond with the tag space
    heap_end = tag_space_start;
}

inline struct mem_tag* add_tag(void* start, void* end, struct mem_tag* prev_tag) {
    // Define the new tag
    struct mem_tag* new_tag = tag_space_end;
    // Find a free spot for the new tag
    do {
        new_tag--;
        if (new_tag < tag_space_start) {
            // Tag space is full. Adding the tag failed.
            return NULL;
        }
        if (new_tag < lowest_tag || lowest_tag == NULL) {
            lowest_tag = new_tag;
            resize_tag_space();
            break;
        }
    } while (new_tag->end != NULL);

    new_tag->start = start;
    new_tag->end = end;

    if (prev_tag == NULL) {
        new_tag->next = first_tag;
        first_tag = new_tag;
    } else {
        new_tag->next = prev_tag->next;
        prev_tag->next = new_tag;
    }
    if (new_tag->next == NULL) {
        last_tag = new_tag;
    }
    return new_tag;
}

inline void remove_tag(struct mem_tag* removed_tag, struct mem_tag* prev_tag) {
    // Change the pointer of the previous tag or the first tag pointer to point towards the next tag after te deleted tag.
    if (prev_tag == NULL) {
        first_tag = removed_tag->next;
    } else {
        prev_tag->next = removed_tag->next;
    }
    // Mark memory area unused by setting the end attribute to null.
    removed_tag->end = NULL;
    // Check if the tag was the lowest, and if so, find the new lowest tag.
    if (removed_tag == lowest_tag) {
        do {
            lowest_tag++;
            if (lowest_tag >= tag_space_end) {
                lowest_tag = NULL;
                break;
            }
        } while (lowest_tag->end == NULL);
        resize_tag_space();
    }
}

inline struct mem_tag* split_tag(void* start, void* end, struct mem_tag* splitted_tag) {
    // Add a new tag to mark the upper part memory marked by the splitted tag. 
    struct mem_tag* new_tag = add_tag(end, splitted_tag->end, splitted_tag);

    // Check if adding the memory tag sucseed, and if not return NULL.
    if (new_tag == NULL) {
        return NULL;
    }

    // Lower the end of the splitted tag to only mark the lower part memory previously marked by the splitted tag. 
    splitted_tag->end = start;
    return new_tag;
}

inline void merge_tag(struct mem_tag* first_tag) {
    // Define the tag to remove in the merge.
    struct mem_tag* second_tag = first_tag->next;
    // Merge the tags by setting first tag to overlap both tags. 
    first_tag->end = second_tag->end;
    // Remove the now unused seconds tag.
    remove_tag(second_tag, first_tag);
}

void* malloc(size_t requested_size) {
    // Define the actual needed size.
    size_t actual_size = requested_size + sizeof(size_t);

    // Define pointers to the end of the requested memory.
    void* start_ptr = heap_start;
    void* end_ptr = heap_start + actual_size;

    struct mem_tag* cur_tag = first_tag;
    struct mem_tag* prev_tag = NULL;

    // If no memory is currently allocated then just check if the actual size fits inside the heap (minus the buffer to be.) 
    if (cur_tag == NULL) {
        if (end_ptr <= heap_end - sizeof(struct mem_tag)) {
            add_tag(start_ptr, end_ptr, NULL);
            goto found_memory;
        } else {
            return NULL;
        }
    }
    // Check if there is enough space before the first tagged space.
    if (end_ptr <= cur_tag->start) {
        if (end_ptr == cur_tag->start) {
            cur_tag->start = start_ptr;
        } else {
            add_tag(start_ptr, end_ptr, NULL);
        }
        goto found_memory;
    }
    // Check if there is enough space between some tagged spaces.
    prev_tag = cur_tag;
    cur_tag = cur_tag->next;
    while (cur_tag != NULL) {
        start_ptr = prev_tag->end;
        end_ptr = prev_tag->end + actual_size;
        if (end_ptr <= cur_tag->start) {
            if (end_ptr == cur_tag->start) {
                merge_tag(prev_tag);
            } else {
                prev_tag->end = end_ptr;
            }
            goto found_memory;
        }
        prev_tag = cur_tag;
        cur_tag = cur_tag->next;
    }
    // Check if there is enough space after the last tagged space.
    start_ptr = prev_tag->end;
    end_ptr = prev_tag->end + actual_size;
    if (end_ptr <= heap_end) {
        prev_tag->end = end_ptr;
        goto found_memory;
    } else {
        return NULL;
    }
    // Define a label to prepare the memory for usage and return it.
    found_memory:
    *(size_t*)(start_ptr) = requested_size;
    return start_ptr + sizeof(size_t);
}

void free(void* ptr) {
    
}

void print_heap()
{
    struct mem_tag* cur_tag = first_tag;
    // Loop through all bytes in the heap and the tag space, (assuming the tag space comes directly after the heap.)
    for (uint8_t* ptr = heap_start; (void*)ptr < (void*)(tag_space_end); ptr++) {
        // Set background color to represent the memory usage.
        if ((void*)ptr < (void*)heap_end) {
            check_memory_is_tagged:
            if (cur_tag == NULL || (void*)ptr < cur_tag->start) {
                // The memory is free
                terminal_setcolor(color_white, color_green);
            } else if ((void*)ptr >= cur_tag->end) {
                cur_tag = cur_tag->next;
                goto check_memory_is_tagged;
            } else {
                // The Memory is used
                terminal_setcolor(color_white, color_red);
            }
        } else {
            // The memory is reserved for tags
            terminal_setcolor(color_white, color_light_blue);
        }
        
        // Write two heximal chars representing the current byte.
        uint8_t cur_heximal = ((*ptr >> 4) & 0x0F); 
        terminal_writechar((cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0');
        cur_heximal = (*ptr & 0x0F);
        terminal_writechar((cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0');
        // Write a space or enter once in a while to keep the output readable.
        if (((void*)ptr - heap_start) % 32 == 31) {
            terminal_setcolor(color_white, color_black);
            terminal_writechar('\n');
        } else if (((void*)ptr - heap_start) % 4 == 3) {
            terminal_setcolor(color_white, color_black);
            terminal_writechar(' ');
        }
    }
    // Reset output color.
    terminal_setcolor(color_white, color_black);
}

void test_heap()
{    
    print_heap();
    uint32_t* a = malloc(4);
    if (a != NULL) *a = 0xAAAAAAAA;
    print_heap();
    uint32_t* b = malloc(4);
    if (b != NULL) *b = 0xBBBBBBBB;
    print_heap();
}
