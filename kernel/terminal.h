#ifndef _TERMINAL_H
#define _TERMINAL_H

enum vga_color {
    color_black,
    color_blue,
    color_green,
    color_cyan,
    color_red,
    color_magenta,
    color_brown,
    color_light_grey,
    color_dark_grey,
    color_light_blue,
    color_light_green,
    color_light_cyan,
    color_light_red,
    color_light_magenta,
    color_light_brown,
    color_white
};

void terminal_setcolor(enum vga_color fg, enum vga_color bg);
void terminal_reset();
void terminal_writechar(char c);
void terminal_writestring(char* str);
void terminal_updatecursor();

#endif
