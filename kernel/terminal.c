#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "ioport.h"

#include "terminal.h"

uint16_t* buffer = (uint16_t*)0xB8000;
static const int width = 80;
static const int height = 25;
int pos_x = 0;
int pos_y = 0;
uint8_t color;

void set_color(enum vga_color fg, enum vga_color bg)
{
    color = fg | bg << 4;
}

void set_entry(int pos, char c)
{
    buffer[pos] = c | color << 8;
}

void update_cursor()
{
    int pos = pos_y * width + pos_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos);
    outb(0x3D4, 0x0E);
    outb(0x3D5, pos >> 8);
}

void term_clear()
{
    pos_x = 0;
    pos_y = 0;
    update_cursor();
    set_color(white, black);

    static const int total = width * height;
    for (int pos = 0; pos < total; pos++) {
        set_entry(pos, '.');
    }
}

void writechar(char c)
{
    set_entry(pos_y * width + pos_x++, c);
}

void writestring(char* str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++){
        writechar(str[i]);
    }
}
