#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "terminal.h"
#include "vga.h"
#include "../malloc.h"

static size_t write_offset = 0;
static size_t write_line = 0;

static size_t render_column = 0;
static size_t render_line = 0;

void write_char(char c)
{
    switch (c) {
        case '\n':
            write_line++;
        case '\r':
            write_offset = 0;
            break;
        default:
            put_char(c);
            break;
    }
}

void write_string(const char* str)
{
    while (*str) {
        write_char(*str);
        str++;
    }
}

/*void put_char(char c)
{
    vga_set_char(VGA_WIDTH*render_line + render_column, c);
    render_column++;
    if (render_column >= VGA_WIDTH) {
        render_line++;
        render_column = 0;
    }
}*/

void put_char(char c)
{
    line_descriptor* line = forced_get_line(write_line);

    if (line == NULL) {debug_string[0]='g'; return;}  // couldn't get the line

    if (write_offset >= line->saving_length) {
        if (resize_line(line, write_offset + 1)) {debug_string[1]='r'; return;}  // couldn't resize the line
    }

    // TODO: find out why this only works with the same command double
    line->string[write_offset] = c;
    line->string[write_offset] = c;

    line->display_length++;
    line->saving_length++;

    write_offset++;
}

void put_string(const char* str)
{
    while (*str) {
        put_char(*str);
        str++;
    }
}
