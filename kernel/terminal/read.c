#include <stddef.h>
#include <stdbool.h>

#include "terminal.h"
#include "vga.h"

static size_t terminal_width = VGA_WIDTH;
static size_t terminal_height = VGA_HEIGHT;

static size_t view_column = 0;
static size_t view_line = 0;

static bool line_wrapping = false;

char debug_string[25] = { 0 };

void update_view()
{
    size_t read_column = 0;
    size_t read_line = 0;

    if (line_wrapping) {
        // TODO: add line wrapping support
    } else {
        read_column = view_column;
        read_line = view_line;
    }

    line_descriptor* line = get_line(read_line);

    for (size_t render_line = 0; render_line < terminal_height; render_line++) {
        for (size_t render_column = 0; render_column < terminal_width; render_column++) {
            if (line != NULL && read_column < line->saving_length) {
                vga_set_char(terminal_width*render_line + render_column, line->string[read_column]);
                read_column++;
            } else {
                vga_set_char(terminal_width*render_line + render_column, '\0');
            }
        }

        /*if (line == NULL) {
            vga_set_char(terminal_width*(render_line+1) - 1, 'N');
        } else {
            vga_set_char(terminal_width*(render_line+1) - 5, line->string[0]);
            vga_set_char(terminal_width*(render_line+1) - 4, '0'+line->allocated_length);
            vga_set_char(terminal_width*(render_line+1) - 3, '0'+line->saving_length);
            vga_set_char(terminal_width*(render_line+1) - 2, '0'+line->display_length);
            vga_set_char(terminal_width*(render_line+1) - 1, 'P');
        }*/

        if (line_wrapping) {
            // TODO: add line wrapping newline
        } else {
            read_line++;
            read_column = 0;  // TODO: add view column stuff
            line = get_line(read_line);
        }
    }

    /*for (size_t i = 0; i < 25; i++) {
        vga_set_char(terminal_width*terminal_height - terminal_width + i, debug_string[i]);
    }*/
}
