#ifndef __vga_h__
#define __vga_h__

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_DEFAULT_FG_COLOR white
#define VGA_DEFAULT_BG_COLOR black

enum vga_color {
    black,
    blue,
    green,
    cyan,
    red,
    magenta,
    brown,
    light_grey,
    dark_grey,
    light_blue,
    light_green,
    light_cyan,
    light_red,
    light_magenta,
    light_brown,
    white,
};

union vga_style {
    struct {
        enum vga_color fg: 4;
        enum vga_color bg: 4;
    };
    uint8_t value;
};

extern union vga_style vga_current_style;

void vga_set_char(int pos, char c);
char vga_get_char(int pos);
void vga_update_cursor(int x, int y);
void vga_clear();
void vga_scroll(int dist);

#endif
