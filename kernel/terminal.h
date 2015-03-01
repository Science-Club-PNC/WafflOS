#ifndef __terminal_h__
#define __terminal_h__

#include <stdint.h>

enum vga_color {
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
};
#define DEFAULT_FG_COLOR vga_white
#define DEFAULT_BG_COLOR vga_black

union {
    struct {
        enum vga_color fg: 4;
        enum vga_color bg: 4;
    };
    uint8_t value;
} vga_color;

void update_cursor();
void term_clear();
void backspace();
void newline();

void putchar(char c);
void writechar(char c);
void writestring(char* str);

#endif
