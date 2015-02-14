#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "vga.h"
#include "../ioport.h"

vga_style vga_current_style;

static uint16_t* buffer = (uint16_t*)0xB8000;

void vga_set_char(int pos, char c)
{
    buffer[pos] = c | (vga_current_style.value) << 8;
}

char vga_get_char(int pos)
{
    return (char)buffer[pos];
}

void vga_update_cursor(int x, int y)
{
    int pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos);
    outb(0x3D4, 0x0E);
    outb(0x3D5, pos >> 8);
}

void vga_clear()
{
    vga_update_cursor(0, 0);
    vga_current_style.fg = VGA_DEFAULT_FG_COLOR;
    vga_current_style.bg = VGA_DEFAULT_BG_COLOR;

    static const int total = VGA_WIDTH * VGA_HEIGHT;
    for (int pos = 0; pos < total; pos++) {
        vga_set_char(pos, '\0');
    }
}

