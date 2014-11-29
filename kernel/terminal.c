#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "terminal.h"

uint16_t *vga_buffer = (uint16_t *)0xB8000;
uint8_t terminal_color;
static const size_t terminal_width = 80;
static const size_t terminal_height = 25;
size_t terminal_x;
size_t terminal_y;

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color)
{
    return (uint16_t)c | (uint16_t)color << 8;
}

void terminal_setcolor(enum vga_color fg, enum vga_color bg)
{
    terminal_color = make_color(fg, bg);
}

void terminal_reset()
{
    terminal_x = 0;
    terminal_y = 0;
    terminal_setcolor(color_white, color_black);

    size_t terminal_size = terminal_width * terminal_height;
    for (size_t pos = 0; pos < terminal_size; pos++) {
        vga_buffer[pos] = make_vgaentry(' ', terminal_color);
    }
}

void terminal_writechar(char c)
{
    if (c == '\n') {
        goto newline;
    }
    vga_buffer[terminal_y * terminal_width + terminal_x] = make_vgaentry(c, terminal_color);

    if (++terminal_x > terminal_width) {
newline:
        terminal_x = 0;

        if (++terminal_y > terminal_height) {
            size_t terminal_size = terminal_width * terminal_height;
            for (size_t pos = terminal_width; pos < terminal_size; pos++) {
                vga_buffer[pos - terminal_width] = vga_buffer[pos];
            }
            for (size_t pos = terminal_width * terminal_height - 1; pos < terminal_size; pos++) {
                vga_buffer[pos] = make_vgaentry(' ', make_color(color_white, color_black));
            }

            terminal_y = terminal_height - 1;
        }
    }
}

void terminal_writestring(char *str)
{
    size_t str_size = strlen(str);
    for (size_t i = 0; i < str_size; i++) {
        terminal_writechar(str[i]);
    }
}
