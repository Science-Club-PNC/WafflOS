#include <stddef.h>
#include <kernel/terminal.h>
#include "string.h"
#include "vararg.h"

#include "io.h"

#define ERRCHAR '?'
#define MAXINT 12

int printf(char *str, ...)
{
    va_list *args;
    va_start(args, str);
    char i[MAXINT];

    size_t pos;
    for (pos = 0; str[pos]; pos++) {
        if (str[pos] == '%') {
            switch (str[++pos]) {
                case 's':
                    writestring(va_arg(args, char*));
                    break;
                case 'c':
                    writechar(va_arg(args, char));
                    break;
                case 'd':
                case 'i':
                    writestring(int_to_dec_string(va_arg(args, int), i, MAXINT));
                    break;
                case 'x':
                case 'X':
                    writestring(int_to_hex_string(va_arg(args, int), i, MAXINT));
                    break;
                case '%':
                    writechar('%');
                    break;
                default:
                    writechar(ERRCHAR);
            }
        } else if (str[pos] == '$') {
            char c = str[++pos];
            void (*color_function)(enum vga_color) = set_fg_color;

            if (c == '$') {
                writechar('$');
            } else if (c == 'R') {
                set_color(DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
            } else {
                if (c == '!') {
                    color_function = set_bg_color;
                    c = str[++pos];
                }
                if (c >= '0' && c <= '9') {
                    color_function(c - '0');
                } else if (c >= 'a' && c <= 'f') {
                    color_function(c - 'a' + 10);
                } else if (c >= 'A' && c <= 'F') {
                    color_function(c - 'A' + 10);
                } else if (c == 'r') {
                    if (color_function == set_fg_color) {
                        set_fg_color(DEFAULT_FG_COLOR);
                    } else {
                        set_bg_color(DEFAULT_BG_COLOR);
                    }
                } else {
                    writechar(ERRCHAR);
                }
            }
        } else {
            writechar(str[pos]);
        }
    }

    update_cursor();
    return pos;
}
