#include <stddef.h>
#include <memory.h>

#include "terminal.h"
#include "../malloc.h"

size_t line_array_length;
line_descriptor* line_array = NULL;

size_t last_line = 0;

int init_line_array(size_t length)
{
    destroy_line_array();

    line_array = calloc(length, sizeof(line_descriptor));
    if (line_array == NULL) return 1;  // couldn't allocate memory for the line array
    line_array_length = length;

    last_line = 0;

    return 0;
}

void destroy_line_array()
{
    if (line_array == NULL) return;

    free(line_array);
    line_array_length = 0;

    last_line = 0;
}

int resize_line_array(size_t length)
{
    if (line_array == NULL) return init_line_array(length);

    line_descriptor* new_line_array = realloc(line_array, length * sizeof(line_descriptor));
    if (new_line_array == NULL) return 1;  // couldn't reallocate memory for the line array
    line_array = new_line_array;
    line_array_length = length;

    return 0;
}

line_descriptor* get_line(size_t line_number)
{
    if (line_number > last_line || line_number >= line_array_length) return NULL;

    return &line_array[line_number];
}

line_descriptor* forced_get_line(size_t line_number)
{
    if (line_number >= line_array_length) {
        if (resize_line_array(line_number + 1)) return NULL;  // couldn't resize the line array
    }

    if (line_number > last_line) {
        last_line = line_number;
    }

    return &line_array[line_number];
}

int init_line(line_descriptor* line, size_t length)
{
    destroy_line(line);

    line->string = malloc(length);
    if (line->string == NULL) return 1;  // couldn't allocate momory for the string
    line->allocated_length = length;

    line->saving_length = 0;
    line->display_length = 0;

    return 0;
}

void destroy_line(line_descriptor* line)
{
    if (line->string == NULL) return;

    free(line->string);
    line->allocated_length = 0;

    line->saving_length = 0;
    line->display_length = 0;
}

int resize_line(line_descriptor* line, size_t length)
{
    if (line->string == NULL) return init_line(line, length);

    char* new_string = realloc(line->string, length);
    if (new_string == NULL) return 1;  // couldn't reallocate memory for the string
    line->string = new_string;
    line->allocated_length = length;

    return 0;
}
size_t get_line_offset(line_descriptor* line, size_t column)
{
    if (line == NULL) return 0;

    size_t offset = 0;
    for (size_t i = 0; i < column; i++) {
        while (line->string[offset] == '\0') {
            offset += 2;
            if (offset >= line->saving_length) {
                return line->saving_length;
            }
        }

        offset++;
        if (offset >= line->saving_length) {
            return line->saving_length;
        }
    }

    return offset;
}
