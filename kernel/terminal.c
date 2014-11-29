#include <stddef.h>
#include <stdint.h>

#include "terminal.h"

uint16_t *vga_buffer = (uint16_t *)0xB8000;
uint8_t terminal_color;
static const size_t terminal_width = 80;
static const size_t terminal_height = 25;
size_t terminal_x;
size_t terminal_y;

void terminal_setcolor(enum vga_color fg, enum vga_color bg)
{
    terminal_color = fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color)
{
    return (uint16_t)c | (uint16_t)color << 8;
}

void terminal_reset()
{
    terminal_x = 0;
    terminal_y = 0;
    terminal_setcolor(color_white, color_green);

    static const size_t terminal_size = terminal_width * terminal_height;
    for (size_t pos = 0; pos < terminal_size; pos++) {
        vga_buffer[pos] = make_vgaentry(' ', terminal_color);
    }
}
