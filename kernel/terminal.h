#ifndef __terminal_h__
#define __terminal_h__

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

void set_color(enum vga_color fg, enum vga_color bg);
void update_cursor();
void term_clear();
void writestring(char* str);

#endif
