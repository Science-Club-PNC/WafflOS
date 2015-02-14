#include <stddef.h>
#include <stdbool.h>

#include "terminal.h"
#include "vga.h"

static size_t terminal_width = VGA_WIDTH;
static size_t terminal_height = VGA_HEIGHT;

static size_t view_column = 0;
static size_t view_line = 0;

static bool line_wrapping = false;

terminal_color default_foreground = white;
terminal_color default_background = black;

void handle_style_char(char c)
{
    alt_char alt_c;
    alt_c.c = c;

    switch (alt_c.type) {
        case front_color:
            vga_current_style.fg = alt_c.value;
            break;
        case back_color:
            vga_current_style.bg = alt_c.value;
            break;
        case style_reset:
            switch (alt_c.value) {
                case front_color:
                    vga_current_style.fg = default_foreground;
                    break;
                case back_color:
                    vga_current_style.bg = default_background;
                    break;
                case style_reset:
                    vga_current_style.fg = default_foreground;
                    vga_current_style.bg = default_background;
                    break;
            }
        break;
    }
}

void fetch_style(line_descriptor* line, size_t column)
{
    if (line == NULL) {
        vga_current_style.fg = default_foreground;
        vga_current_style.bg = default_background;
    } else {
        if (line->start_style.defined_foreground) {
            vga_current_style.fg = line->start_style.fg;
        } else {
            vga_current_style.fg = default_foreground;
        }

        if (line->start_style.defined_background) {
            vga_current_style.bg = line->start_style.bg;
        } else {
            vga_current_style.bg = default_background;
        }

        for (size_t i = 0; i < column && i < line->saving_length; i++) {
            if (line->string[i] == '\0') {
                i++;
                handle_style_char(line->string[i]);
            }
        }
    }
}

void update_view()
{
    size_t read_offset = 0;
    size_t read_line = 0;

    if (line_wrapping) {
        // TODO: add line wrapping support
    } else {

        read_line = view_line;
    }

    line_descriptor* line = get_line(read_line);

    if (line_wrapping) {
        // TODO: add line wrapping support
    } else {
        read_offset = get_line_offset(line, view_column);
    }

    fetch_style(line, read_offset);

    for (size_t render_line = 0; render_line < terminal_height; render_line++) {
        for (size_t render_column = 0; render_column < terminal_width; render_column++) {
            char char_to_render = '\0';

            if (line != NULL) {
                while (read_offset < line->saving_length) {
                    char c = line->string[read_offset];
                    read_offset++;

                    if (c == '\0') {
                        handle_style_char(line->string[read_offset]);
                        read_offset++;
                    } else {
                        char_to_render = c;
                        break;
                    }
                }
            }
            vga_set_char(terminal_width*render_line + render_column, char_to_render);
        }

        if (line_wrapping) {
            // TODO: add line wrapping newline
        } else {
            read_line++;
            line = get_line(read_line);
            read_offset = get_line_offset(line, view_column);
            fetch_style(line, read_offset);
        }
    }
}
