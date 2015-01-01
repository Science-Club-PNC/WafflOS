#include <stddef.h>
#include <stdbool.h>
#include <kernel/terminal.h>
#include "string.h"
#include "vararg.h"

#include "io.h"

#define set_color(bg, value) if (bg) { vga_color.bg = value; } else { vga_color.fg = value; }

#define ERRCHAR '?'
#define MAXINT 10

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
                case 'i':
                    writestring(int_to_dec_string(va_arg(args, int), i, MAXINT));
                    break;
                case '%':
                    writechar('%');
                    break;
                default:
                    writechar(ERRCHAR);
            }
        } else if (str[pos] == '$') {
            char c = str[++pos];
            bool bg = false;

            if (c == '$') {
                writechar('$');
            } else if (c == 'R') {
                vga_color.fg = DEFAULT_FG_COLOR;
                vga_color.bg = DEFAULT_BG_COLOR;
            } else {
                if (c == '!') {
                    bg = true;
                    c = str[++pos];
                }
                if (c >= '0' && c <= '9') {
                    set_color(bg, c - '0');
                } else if (c >= 'a' && c <= 'f') {
                    set_color(bg, c - 'a' + 10);
                } else if (c >= 'A' && c <= 'F') {
                    set_color(bg, c - 'A' + 10);
                } else if (c == 'r') {
                    if (bg) {
                        vga_color.bg = DEFAULT_BG_COLOR;
                    } else {
                        vga_color.fg = DEFAULT_FG_COLOR;
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
