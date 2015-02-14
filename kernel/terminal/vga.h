#ifndef __vga_h__
#define __vga_h__

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_DEFAULT_FG_COLOR vga_white
#define VGA_DEFAULT_BG_COLOR vga_black

typedef enum {
    vga_black,
    vga_blue,
    vga_green,
    vga_cyan,
    vga_red,
    vga_magenta,
    vga_brown,
    vga_light_grey,
    vga_dark_grey,
    vga_light_blue,
    vga_light_green,
    vga_light_cyan,
    vga_light_red,
    vga_light_magenta,
    vga_light_brown,
    vga_white,
} vga_color;

typedef union {
    struct {
        vga_color fg: 4;
        vga_color bg: 4;
    };
    uint8_t value;
} vga_style;

extern vga_style vga_current_style;

void vga_set_char(int pos, char c);
char vga_get_char(int pos);
void vga_update_cursor(int x, int y);
void vga_clear();
void vga_scroll(int dist);

#endif
