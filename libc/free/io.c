#include <stddef.h>
#include <kernel/terminal.h>
#include "string.h"
#include "vararg.h"

#include "io.h"

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
                    writestring(int_to_string(va_arg(args, int), i, MAXINT));
                    break;
                case 'r':
                    set_color(DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
                    break;
                case '%':
                    writechar('%');
                    break;
                default:
                    writechar('?');
            }
        } else if (str[pos] == '&' || str[pos] == '$') {
            // http://minecraft.gamepedia.com/Formatting_codes#Color_codes
            char c = str[pos];
            int i = str[++pos];

            if (i == c) {
                writechar(c);
            } else if (i >= 48 && i <= 57) {
                if (c == '&') {
                    set_fg_color(i - 48);
                } else if (c == '$') {
                    set_bg_color(i - 48);
                }
            } else if (i >= 97 && i <= 102) {
                if (c == '&') {
                    set_fg_color(i - 87);
                } else if (c == '$') {
                    set_bg_color(i - 87);
                }
            } else if (i == 'r') {
                if (c == '&') {
                    set_fg_color(DEFAULT_FG_COLOR);
                } else if (c == '$') {
                    set_bg_color(DEFAULT_BG_COLOR);
                }
            }
        } else {
            writechar(str[pos]);
        }
    }

    update_cursor();
    return pos;
}
