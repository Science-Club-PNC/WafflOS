#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <kernel/terminal.h>

#include "io.h"
#include "string.h"

#define set_color(bg, value) if (bg) { vga_color.bg = value; } else { vga_color.fg = value; }

#define ERRCHAR '?'
#define NUMBER_STRING_LENGTH 20

struct format {
    enum {
        right_justify,
        left_justify,
        zero_justify,
        center_justify
    } justify_mode;
    char sign_fill_in;
    bool add_type_prefix;
    size_t minimal_output_width;

    int data_size;
    enum {
        text,
        decimal,
        heximal,
        octal
    } specifier_type;

    char* output_string_ptr;
    char* output_prefix_ptr;
};

const char* handle_color_code(const char* c, int* output_count);
const char* handle_format_code(const char* c, int* output_count, va_list* args);

const char* get_format_flags(const char* c, struct format* format);
const char* get_format_width(const char* c, struct format* format, va_list* args);
const char* get_format_data_size(const char* c, struct format* format);
const char* get_format_output(const char* c, struct format* format, va_list* args);
int print_format_output(struct format* format, int* output_count);


int printf(const char* str, ...)
{
    va_list args;
    va_start(args, str);

    const char* c = str;
    int output_count = 0;
    while (*c) {
        if (*c == '$') {
            c++;
            if (*c == '$') {
                writechar('$');
                output_count++;
            } else {
                c = handle_color_code(c, &output_count);
                if (!*c) break;
            }
        } else if (*c == '%') {
            c++;
            if (*c == '%') {
                writechar('%');
                output_count++;
            } else {
                c = handle_format_code(c, &output_count, &args);
                if (!*c) break;
            }
        } else {
            writechar(*c);
            output_count++;
        }

        c++;
    }

    update_cursor();
    return output_count;
}

const char* handle_color_code(const char* c, int* output_count)
{
    bool bg = false;

    if (*c == 'R') {
        vga_color.fg = DEFAULT_FG_COLOR;
        vga_color.bg = DEFAULT_BG_COLOR;
    } else {
        if (*c == '!') {
            bg = true;
            c++;
        }
        if (*c >= '0' && *c <= '9') {
            set_color(bg, *c - '0');
        } else if (*c >= 'a' && *c <= 'f') {
            set_color(bg, *c - 'a' + 10);
        } else if (*c == 'r') {
            if (bg) {
                vga_color.bg = DEFAULT_BG_COLOR;
            } else {
                vga_color.fg = DEFAULT_FG_COLOR;
            }
        } else {
            writechar(ERRCHAR);
            ++*output_count;
        }
    }
    return c;
}

const char* handle_format_code(const char* c, int* output_count, va_list* args)
{
    struct format format;

    c = get_format_flags(c, &format);
    c = get_format_width(c, &format, args);
    c = get_format_data_size(c, &format);

    char number_string[NUMBER_STRING_LENGTH] = "";
    char prefix_string[4] = "";

    format.output_string_ptr = number_string;
    format.output_prefix_ptr = prefix_string;

    c = get_format_output(c, &format, args);
    print_format_output(&format, output_count);

    return c;
}

const char* get_format_flags(const char* c, struct format* format)
{
    // Set default flag variables.
    format->justify_mode = right_justify;
    format->sign_fill_in = '\0';
    format->add_type_prefix = false;

    // Get flags
    while (true) {
        switch (*c) {
            case '-':
                format->justify_mode = left_justify;
                break;
            case '0':
                format->justify_mode = zero_justify;
                break;
            case ' ':
                format->sign_fill_in = ' ';
                break;
            case '+':
                format->sign_fill_in = '+';
                break;
            case '#':
                format->add_type_prefix = true;
                break;
            default:
                // Current char isn't a flag, return.
                return c;
        }
        c++;
    }
}

const char* get_format_width(const char* c, struct format* format, va_list* args)
{
    // Set default minimal output width
    format->minimal_output_width = 0;

    // Get minimal output width
    if (*c == '*') {
        format->minimal_output_width = va_arg(*args, int);
        c++;
    } else {
        const char* width_string = c;
        size_t width_string_length = 0;
        while (*c >= '0' && *c <= '9') {
            width_string_length++;
            c++;
        }
        if (width_string_length > 0) {
            format->minimal_output_width = dec_string_to_int(width_string, width_string_length);
        }
    }

    return c;
}

const char* get_format_data_size(const char* c, struct format* format)
{
    // Set default data size
    format->data_size = 0;  // Data is int sized (if using integer specifier)

    // Get data size
    if (*c == 'h') {
        c++;
        if (*c == 'h') {
            format->data_size = -2;  // Data is char sized.
            c++;
        } else {
            format->data_size = -1;  // Data is short sized.
        }
    } else if (*c == 'l') {
        c++;
        if (*c == 'l') {
            format->data_size = 2;  // Data is long long sized.
            c++;
        } else {
            format->data_size = 1;  // Data is long sized.
        }
    }

    return c;
}

const char* get_format_output(const char* c, struct format* format, va_list* args)
{
    switch (*c) {
        case 's':
            // String
            format->specifier_type = text;

            switch(format->data_size) {
                case 0:
                    format->output_string_ptr = va_arg(*args, char*);
                    break;
                default:
                    goto error;
            }

            break;

        case 'c':
            // Character
            format->specifier_type = text;

            switch(format->data_size) {
                    case 0:
                    format->output_string_ptr[0] = va_arg(*args, int);
                    break;
                default:
                    goto error;
            }

            break;

        case 'd':
        case 'i':
            // Signed decimal integer
            format->specifier_type = decimal;

            switch(format->data_size) {
                case -2:
                case -1:
                    format->output_string_ptr = short_to_dec_string(va_arg(*args, int), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 0:
                    format->output_string_ptr = int_to_dec_string(va_arg(*args, int), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 1:
                    format->output_string_ptr = long_to_dec_string(va_arg(*args, long), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                default:
                    goto error;
            }

            if (format->output_string_ptr[0] == '-') {
                format->output_prefix_ptr[0] = '-';
                format->output_string_ptr++;
            } else if (format->sign_fill_in) {
                format->output_prefix_ptr[0] = format->sign_fill_in;
            }
            break;

        case 'u':
            // Unsigned decimal integer
            format->specifier_type = decimal;

            switch(format->data_size) {
                case -2:
                case -1:
                    format->output_string_ptr = ushort_to_dec_string(va_arg(*args, unsigned int), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 0:
                    format->output_string_ptr = uint_to_dec_string(va_arg(*args, unsigned int), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 1:
                    format->output_string_ptr = ulong_to_dec_string(va_arg(*args, unsigned long), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                default:
                    goto error;
            }

            if (format->sign_fill_in) {
                format->output_prefix_ptr[0] = format->sign_fill_in;
            }
            break;

        case 'x':
        case 'X':
            // Unsigned heximal integer
            format->specifier_type = heximal;

            switch(format->data_size) {
                case -2:
                    format->output_string_ptr = char_to_hex_string(va_arg(*args, unsigned int), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case -1:
                    format->output_string_ptr = short_to_hex_string(va_arg(*args, unsigned int), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 0:
                    format->output_string_ptr = int_to_hex_string(va_arg(*args, unsigned int), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 1:
                    format->output_string_ptr = long_to_hex_string(va_arg(*args, unsigned long), format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                default:
                    goto error;
            }

            if (*c == 'X') {
                if (format->add_type_prefix) {
                    format->output_prefix_ptr[0] = '0';
                    format->output_prefix_ptr[1] = 'X';
                }
                to_uppercase(format->output_string_ptr, NUMBER_STRING_LENGTH);
            } else {
                if (format->add_type_prefix) {
                    format->output_prefix_ptr[0] = '0';
                    format->output_prefix_ptr[1] = 'x';
                }
            }
            break;
        default:
            // Unknown type
            goto error;
    }
    return c;

    error:
    format->output_string_ptr = NULL;
    return c;
}

int print_format_output(struct format* format, int* output_count)
{
    if (format->output_string_ptr == NULL)  goto error;

    size_t output_string_length = strlen(format->output_string_ptr) + strlen(format->output_prefix_ptr);

    // Print spaces to justify right if that's the format
    if (format->justify_mode == right_justify) {
        while (output_string_length < format->minimal_output_width) {
            writechar(' ');
            output_string_length++;
        }
    }

    // Print prefix
    writestring(format->output_prefix_ptr);

    // Print zero's to justify if that's the format
    if (format->justify_mode == zero_justify) {
        while (output_string_length < format->minimal_output_width) {
            writechar('0');
            output_string_length++;
        }
    }

    // Print output
    writestring(format->output_string_ptr);

    // Print spaces to justify left if that's the format
    if (format->justify_mode == left_justify) {
        while (output_string_length < format->minimal_output_width) {
            writechar(' ');
            output_string_length++;
        }
    }

    *output_count += output_string_length;
    return 0;

    error:
    writechar(ERRCHAR);
    ++*output_count;
    return 1;
}
