#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <io.h>
#include <memory.h>

#include "malloc.h"
#include "terminal/terminal.h"

// Define struct to point towards the end and start of used memory
struct mem_tag{
    void* start;
    void* end;
    struct mem_tag* next;
};

// Add an array to the stack to act as heap.
// TODO: Create the heap dynamicly relative to the available size.
#define heap_size 0x100000  // you might want to change this value to something like 0x20 for debuging
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
    // Set the buffer size.
    tag_space_buffer_size = 1;

    // Set the start and end of the tag space.
    tag_space_end = (struct mem_tag*)(heap_array + heap_size);
    tag_space_start = tag_space_end - tag_space_buffer_size;

    // Set the start and end of the heap.
    heap_start = (void*)heap_array;
    heap_end = (void*)tag_space_start;

    // Since there are no tags yet, set all tag pointers to NULL.
    first_tag = NULL;
    last_tag = NULL;
    lowest_tag = NULL;
}

// Resizes the tag space to fit all tags plus the tag space buffer.
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
        tag_space_start = last_tag->end;
    }
    // Resize the heap to correspond with the tag space
    heap_end = tag_space_start;
}

// Adds a memory tag that tags the memory from start to end. In the linked list of tags it will come after prev_tag. If prev_tag is NULL then the new tag will be made the first tag. Returs NULL if it failed to add the tag.
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
            // The new tag is the new lowest tag, thus the tag space needs to be resized.
            lowest_tag = new_tag;
            resize_tag_space();
            break;
        }
    // If this tag spot is marked empty stop the loop.
    } while (new_tag->end != NULL);

    // Set start and end mark on the new tag.
    new_tag->start = start;
    new_tag->end = end;

    // Manage the ordering to make the new tag fit in.
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

// Removes the memory tag pointed towards with removed_tag. prev_tag->next should always be removed_tag.
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

// Splits the memory tag pointed towards with removed_tag. start and end define the start and end of the free space between the new tags. returns NULL if it fails to add another memory tag.
inline struct mem_tag* split_tag(void* start, void* end, struct mem_tag* splitted_tag) {
    // Add a new tag to mark the upper part memory marked by the splitted tag.
    struct mem_tag* new_tag = add_tag(end, splitted_tag->end, splitted_tag);

    // Check if adding the memory tag succeed, and if not return NULL.
    if (new_tag == NULL) {
        return NULL;
    }

    // Lower the end of the splitted tag to only mark the lower part memory previously marked by the splitted tag.
    splitted_tag->end = start;

    return new_tag;
}

// Merges the memory tag pointed towards with first_tag and first_tag->next.
inline void merge_tag(struct mem_tag* first_tag) {
    // Define the tag to remove in the merge.
    struct mem_tag* second_tag = first_tag->next;
    // Merge the tags by setting first tag to overlap both tags.
    first_tag->end = second_tag->end;
    // Remove the now unused seconds tag.
    remove_tag(second_tag, first_tag);
}

// Allocates memory of the requested size and returns a pointer towards it. Returns NULL if it fails to allocate the requested amount of memory.
void* malloc(size_t requested_size) {
    // Define the size needed to hold the requested memory plus the size header.
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

// Allocates memory of the requested size times the amount, clears the allocated memory space with zero's and returns a pointer towards it. Returns NULL if it fails to allocate the requested amount of memory.
// TODO: maybe try to allocate the memory in alignment with it's size?
void* calloc(size_t amount, size_t size)
{
    size_t total_size = amount*size;
    void* ptr = malloc(total_size);

    if (ptr == NULL) return NULL;

    memset(ptr, 0, total_size);

    return ptr;
}

void* realloc(void* ptr, size_t requested_size)
{
    if (ptr == NULL) return malloc(requested_size);  // Just let malloc handle this call.
    // if (requested_size == 0) free(ptr); // Just let free handle this call.

    // Find the start and end of the memory to reallocate.
    void* old_start_ptr = ptr - sizeof(size_t);
    size_t old_actual_size = *(size_t*)(old_start_ptr) + sizeof(size_t);
    void* old_end_ptr = old_start_ptr + old_actual_size;

    // Set the start and end of the memory to reallocate if it where to grow/shrink in place.
    void* new_start_ptr = old_start_ptr;
    size_t new_actual_size = requested_size + sizeof(size_t);
    void* new_end_ptr = new_start_ptr + new_actual_size;

    if (new_actual_size == old_actual_size) return ptr;  // Nothing needs to be done

    // Loop through all tag to find in what tagged space the memory to reallocate is.
    struct mem_tag* prev_tag __attribute__((unused)) = NULL;
    struct mem_tag* cur_tag = first_tag;
    while (cur_tag != NULL) {
        if (old_end_ptr <= cur_tag->end) {
            if (old_start_ptr >= cur_tag->start) {
                // Found the tag with the memory to reallocate.
                if (new_actual_size < old_actual_size) {
                    // The memory to reallocate is to shrink.
                    if (old_end_ptr == cur_tag->end) {
                        cur_tag->end = new_end_ptr;
                    } else {
                        if (split_tag(new_end_ptr, old_end_ptr, cur_tag) == NULL)
                            return ptr;  // failed to shrink the memory to reallocate. Just pretend it did sucseed since the program shouldn't be bothered with internals and free() will still correctly free it.
                    }

                    *(size_t*)new_start_ptr = requested_size;
                    return ptr;
                } else {
                    // The memory to reallocate is to grow.
                    if (old_end_ptr == cur_tag->end && (cur_tag->next == NULL ? new_end_ptr <= heap_end : new_end_ptr <= cur_tag->next->start)) {  // Summary: If the memory to reallocate is at the end of the tagged space and there is enough space after the tagged space.
                        if (cur_tag->next != NULL && new_end_ptr == cur_tag->next->start) {
                            merge_tag(cur_tag);
                        } else {
                            cur_tag->end = new_end_ptr;
                        }

                        *(size_t*)new_start_ptr = requested_size;
                        return ptr;
                    } else {
                        // The memory allocator will have to move the memory.
                        void* new_ptr = malloc(requested_size);
                        if (new_ptr == NULL) return NULL;

                        memcpy(new_ptr, ptr, *(size_t*)old_start_ptr);
                        free(ptr);

                        return new_ptr;
                    }
                }
            }

            return NULL;
        }
        prev_tag = cur_tag;
        cur_tag = cur_tag->next;
    }

    return NULL;
}

// Frees the memory pointed towards by ptr. this function may silently fail in certain conditions. this will lead to  a memory leak unless it is tried again under correct conditions.
void free(void* ptr) {
    // Find the start and end of the memory to free.
    void* start_ptr = ptr - sizeof(size_t);
    size_t actual_size = *(size_t*)(start_ptr) + sizeof(size_t);
    void* end_ptr = start_ptr + actual_size;

    // Loop through all tag to find in what tagged space the memory to free is.
    struct mem_tag* prev_tag = NULL;
    struct mem_tag* cur_tag = first_tag;
    while (cur_tag != NULL) {
        if (end_ptr <= cur_tag->end) {
            if (start_ptr >= cur_tag->start) {
                if (start_ptr == cur_tag->start) {
                    if (end_ptr == cur_tag->end) {
                        // The memory to free spans a whole tag, so the tag can be removed.
                        remove_tag(cur_tag, prev_tag);
                    } else {
                        // The memory to free is located at the end of the tagged space, so the end of the tagged space is moved to the begin of the memory to free.
                        cur_tag->start = end_ptr;
                    }
                } else {
                    if (end_ptr == cur_tag->end) {
                        // The memory to free is located at the start of the tagged space, so the end of the tagged space is moved to the begin of the memory to free.
                        cur_tag->end = start_ptr;
                    } else {
                        // The memory to free is located at neither ends of the tagged space, so the tag needs to be split.
                        split_tag(start_ptr, end_ptr, cur_tag);
                    }
                }
            }
            return;
        }
        prev_tag = cur_tag;
        cur_tag = cur_tag->next;
    }
}

// Print the heap and tag space for debuging purpose.
void print_heap()
{
    struct mem_tag* cur_tag = first_tag;

    char byte_string[3] = { 0 };
    // Loop through all bytes in the heap and the tag space, (assuming the tag space comes directly after the heap.)
    for (uint8_t* ptr = heap_start; (void*)ptr < (void*)(tag_space_end); ptr++) {
        // Set background color to represent the memory usage.
        if ((void*)ptr < (void*)heap_end) {
            check_memory_is_tagged:
            if (cur_tag == NULL || (void*)ptr < cur_tag->start) {
                // The memory is free
                printf("$f$!2");
            } else if ((void*)ptr >= cur_tag->end) {
                cur_tag = cur_tag->next;
                goto check_memory_is_tagged;
            } else {
                // The Memory is used
                printf("$f$!4");
            }
        } else {
            // The memory is reserved for tags
            printf("$f$!9");
        }

        // Write two heximal chars representing the current byte.
        uint8_t cur_heximal = (*ptr & 0x0F);
        byte_string[0] = (cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0';
        cur_heximal = ((*ptr >> 4) & 0x0F);
        byte_string[1] = (cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0';
        printf(byte_string);

        // Write a space or enter once in a while to keep the output readable.
        if (((void*)ptr - heap_start) % 32 == 31) {
            printf("$R\n");
        } else if (((void*)ptr - heap_start) % 4 == 3) {
            printf("$R ");
        }
    }
    // Reset output color.
    printf("$R");
}

// A small test function to test if the memory allocator behaves correctly.
void test_heap()
{
    print_heap();
    printf("\n");
    uint32_t* a = malloc(4);
    if (a != NULL) *a = 0xAAAAAAAA;
    uint32_t* b = malloc(4);
    if (b != NULL) *b = 0xBBBBBBBB;
    uint32_t* c = calloc(3, 2);
    if (c != NULL) *c = 0xCCCCCCCC;
    print_heap();
    printf("\n");
    free(b);
    print_heap();
    printf("\n");
    a = realloc(a, 8);
    if (a != NULL) a[1] = 0xDDDDDDDD;
    print_heap();
    printf("\n");
    free(c);
    print_heap();
    printf("\n");
    free(a);
    print_heap();
    printf("\n");
}
