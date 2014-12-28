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
#define DEFAULT_FG_COLOR white
#define DEFAULT_BG_COLOR black

void set_color(enum vga_color fg, enum vga_color bg);
void set_fg_color(enum vga_color fg);
void set_bg_color(enum vga_color bg);
void update_cursor();
void term_clear();
void writechar(char c);
void writestring(char* str);

#endif
