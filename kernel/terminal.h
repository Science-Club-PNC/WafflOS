#ifndef __terminal_h__
#define __terminal_h__

#include <stdint.h>

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
#define DEFAULT_FG_COLOR white
#define DEFAULT_BG_COLOR black

union {
    struct {
        enum vga_color fg: 4;
        enum vga_color bg: 4;
    };
    uint8_t value;
} vga_color;

void update_cursor();
void term_clear();
void writechar(char c);
void writestring(char* str);

#endif
