#include <stddef.h>
#include <stdbool.h>
#include <kernel/terminal.h>
#include "string.h"
#include "vararg.h"

#include "io.h"

#define set_color(bg, value) if (bg) { vga_color.bg = value; } else { vga_color.fg = value; }

#define ERRCHAR '?'
#define NUMBER_STRING_LENGTH 20

int printf(char *str, ...)
{
    va_list *args;
    va_start(args, str);
    char number_string[NUMBER_STRING_LENGTH];

    size_t pos;
    for (pos = 0; str[pos]; pos++) {
        if (str[pos] == '%') {
            // Get flags
            bool left_justify = false;
            char justify_char = ' ';
            bool always_print_sign = false;
            char* sign_replacement = "+";
            char print_numeric_type = false;

            get_next_flag:
            switch (str[++pos]) {
                case '-':
                    left_justify = true;
                    goto get_next_flag;
                case '0':
                    justify_char = '0';
                    goto get_next_flag;
                case ' ':
                    always_print_sign = true;
                    sign_replacement = " ";
                    goto get_next_flag;
                case '+':
                    always_print_sign = true;
                    goto get_next_flag;
                case '#':
                    print_numeric_type = true;
                    goto get_next_flag;
            }

            // Get width
            int minimal_width = 0;
            char* width_string_ptr = &str[pos];
            size_t width_string_length = 0;
            while (str[pos] >= '0' && str[pos] <= '9') {
                width_string_length++;
                pos++;
            }
            if (width_string_length > 0) {
                minimal_width = dec_string_to_int(width_string_ptr, width_string_length);
            }
            // TODO: write precision specifier handeling

            int data_length = 0;  // Data length defaults to int size.
            if (str[pos] == 'h') {
                if (str[++pos] == 'h') {
                    data_length = -2;  // Data is char sized.
                } else {
                    data_length = -1;  // Data is short sized.
                }
            } else if (str[pos] == 'l') {
                data_length = 1;  // Data is long sized.
                pos++;
            }

            // Get specifier
            char* string_to_print = NULL;
            char* string_prefix = "";
            
            switch (str[pos]) {
                case 's':
                    string_to_print = va_arg(args, char*);
                    break;
                case 'c':
                    string_to_print = number_string;
                    string_to_print[0] = va_arg(args, char);
                    string_to_print[1] = '\0';
                    break;
                case 'd':
                case 'i':
                    switch(data_length) {
                        case -1:
                            string_to_print = short_to_dec_string(va_arg(args, short), number_string, NUMBER_STRING_LENGTH);
                            break;
                        case 0:
                            string_to_print = int_to_dec_string(va_arg(args, int), number_string, NUMBER_STRING_LENGTH);
                            break;
                        case 1:
                            string_to_print = long_to_dec_string(va_arg(args, long), number_string, NUMBER_STRING_LENGTH);
                            break;
                    }
                    if (string_to_print[0] == '-') {
                        string_prefix = "-";
                        string_to_print++;
                    } else if (always_print_sign) {
                        string_prefix = sign_replacement;
                    }
                    break;
                case 'x':
                case 'X':
                    switch(data_length) {
                        case -2:
                            string_to_print = char_to_hex_string(va_arg(args, char), number_string, NUMBER_STRING_LENGTH);
                            break;
                        case -1:
                            string_to_print = short_to_hex_string(va_arg(args, short), number_string, NUMBER_STRING_LENGTH);
                            break;
                        case 0:
                            string_to_print = int_to_hex_string(va_arg(args, int), number_string, NUMBER_STRING_LENGTH);
                            break;
                        case 1:
                            string_to_print = long_to_hex_string(va_arg(args, long), number_string, NUMBER_STRING_LENGTH);
                            break;
                    }
                    if (str[pos] == 'X') {
                        if (print_numeric_type) {
                            string_prefix = "0X";
                        }
                        to_uppercase(string_to_print, strlen(string_to_print));
                    } else {
                        if (print_numeric_type) {
                            string_prefix = "0x";
                        }
                    }
                    break;
                case '%':
                    writechar('%');
                    break;
            }
            if (string_to_print) {
                int string_to_print_length = strlen(string_to_print) + strlen(string_prefix);
                if (!left_justify && justify_char == ' ') {
                    while (string_to_print_length < minimal_width) {
                        writechar(justify_char);
                        string_to_print_length++;
                    }
                }

                writestring(string_prefix);

                if (!left_justify && justify_char == '0') {
                    while (string_to_print_length < minimal_width) {
                        writechar(justify_char);
                        string_to_print_length++;
                    }
                }

                writestring(string_to_print);

                if (left_justify && justify_char == ' ') {
                    while (string_to_print_length < minimal_width) {
                        writechar(justify_char);
                        string_to_print_length++;
                    }
                }
            } else {
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
