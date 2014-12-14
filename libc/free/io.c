#include <terminal.h>

#include "vararg.h"
#include "string.h"

#include "io.h"

#define max_int 20

int printf(char* string, ...)
{
    va_list* args;
    char i[max_int];
    va_start(args, string);

    size_t pos;
    for (pos = 0; string[pos]; pos++) {
        if (string[pos] == '%') {
            switch (string[++pos]) {
                case 's':
                    terminal_writestring(va_arg(args, char*));
                    break;
                case 'c':
                    terminal_writechar(va_arg(args, char));
                    break;
                case 'i':
                    terminal_writestring(int_to_string(va_arg(args, int), i, max_int));
                    break;
                case '%':
                    terminal_writechar('%');
                    break;
                default:
                    return -1;
            }
        } else {
            terminal_writechar(string[pos]);
        }
    }

    terminal_updatecursor();
    return pos;
}
