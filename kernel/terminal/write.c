#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>

#include "terminal.h"
#include "vga.h"
#include "../malloc.h"

static size_t write_offset = 0;
static size_t write_line = 0;

bool insert = false;

void place_char(char c);
void place_style(alt_char c);

void write_char(char c)
{
    switch (c) {
        case '\n':
            write_line++;
        case '\r':
            write_offset = 0;
            break;
        case '\t':
            for (int i = 0; i < TAB_WIDTH; i++) {
                place_char(' ');
            }
        default:
            place_char(c);
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

void put_char(char c)
{
    place_char(c);
}

void put_string(const char* str)
{
    while (*str) {
        put_char(*str);
        str++;
    }
}

void set_front_color(int color)
{
    alt_char c;
    c.type = front_color;
    c.value = color;
    place_style(c);
}

void reset_front_color()
{
    alt_char c;
    c.type = style_reset;
    c.value = front_color;
    place_style(c);
}

void set_back_color(int color)
{
    alt_char c;
    c.type = back_color;
    c.value = color;
    place_style(c);
}


void reset_back_color()
{
    alt_char c;
    c.type = style_reset;
    c.value = back_color;
    place_style(c);
}

void reset_style()
{
    alt_char c;
    c.type = style_reset;
    c.value = style_reset;
    place_style(c);
}

void update_style_from_line(size_t line_num)
{
    while (true) {
        line_descriptor* line = get_line(line_num);
        if (line == NULL) return;

        line_descriptor* next_line = get_line(line_num);
        if (next_line == NULL) return;


        terminal_style __attribute__((unused)) style;

        bool found_bg = false;
        bool found_fg = false;

        size_t i = line->saving_length;
        while (!(found_bg & found_fg)) {
            if (i == 0) {
                if (!found_fg) {
                    style.defined_foreground = line->start_style.defined_foreground;
                    style.fg = line->start_style;
                }
            }
            i--;

            if (line->string[i] == '\0') {
                alt_char alt_c;
                alt_c.c = line->string[i+1];

                switch (alt_c.type) {
                case front_color:
                    if (!found_fg) {
                        found_fg = true;
                        style.defined_foreground = true;
                        style.fg = alt_c.value;
                    }
                    break;
                case back_color:
                    if (!found_bg) {
                        found_bg = true;
                        style.defined_background = true;
                        style.bg = alt_c.value;
                    }
                    break;
                case style_reset:
                    switch (alt_c.value) {
                    case front_color:
                        found_fg = true;
                        break;
                    case back_color:
                        found_bg = true;
                        break;
                    case style_reset:
                        found_fg = true;
                        found_bg = true;
                        break;
                    }
                break;
                }
            }
        }

        if (style == next_line->start_style) break;
        next_line->start_style = style;
    }
}

void place_char(char c)
{
    line_descriptor* line = forced_get_line(write_line);

    if (line == NULL) return;  // couldn't get the line

    if (insert || write_offset >= line->saving_length) {
        insert:

        if (line->saving_length >= line->allocated_length) {
            if (resize_line(line, line->saving_length + 1)) return;  // couldn't resize the line
        }

        memmove(&line->string[write_offset + 1], &line->string[write_offset], line->saving_length - write_offset);

        line->saving_length++;
        line->display_length++;
    } else {
        if (write_offset >= line->saving_length) goto insert;

        size_t overwrite_offset = write_offset;
        while (line->string[overwrite_offset] == '\0') {
            overwrite_offset += 2;
            if (overwrite_offset >= line->saving_length) goto insert;
        }

        memmove(&line->string[write_offset + 1], &line->string[write_offset], overwrite_offset - write_offset);
    }

    line->string[write_offset] = c;
    write_offset++;
}

void place_style(alt_char new_style)
{
    line_descriptor* line = forced_get_line(write_line);

    if (line == NULL) return;  // couldn't get the line

    // vars about how and where to place the new style
    bool insert_style = true;
    size_t style_start_offset = write_offset;

    // find a spot to place the new style at
    while (true) {
        if (style_start_offset >= 2) {
            style_start_offset -= 2;

            if (line->string[style_start_offset] == '\0') {  // found an alt char. check if it is for the same type or resets this type.
                alt_char found_style;
                found_style.c = line->string[style_start_offset + 1];

                if ((new_style.type == style_reset) ?
                    (found_style.type == new_style.value || (found_style.type == style_reset && found_style.value == new_style.value)) :
                    (found_style.type == new_style.type || (found_style.type == style_reset && found_style.value == new_style.type))) {

                    insert_style = false;
                    break;
                } else if (found_style.type == style_reset && found_style.value == style_reset) {  // found a complete style reset. return to the normal write location.
                    style_start_offset = write_offset;
                    break;
                }
            } else {  // found a printable character. return to the normal write location.
                style_start_offset = write_offset;
                break;
            }
        } else {  // found the start of the line. return to the normal write location.
            style_start_offset = write_offset;
            break;
        }
    }

    if (insert_style) {
        if (line->saving_length + 2 > line->allocated_length) {
            if (resize_line(line, line->saving_length + 2)) return;  // couldn't resize the line
        }
        memmove(&line->string[style_start_offset + 2], &line->string[style_start_offset], line->saving_length - style_start_offset);
        line->saving_length += 2;
        write_offset += 2;
    }

    line->string[style_start_offset] = '\0';
    line->string[style_start_offset + 1] = new_style.c;
}
