#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include <kernel/terminal.h>

#include "base.h"
#include "io.h"
#include "string.h"

#define set_color(bg, value) if (bg) { vga_color.bg = value; } else { vga_color.fg = value; }

#define ERRCHAR '?'
#define NUMBER_STRING_LENGTH 33

typedef struct {
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
} format;

static const char* handle_color_code(const char* c, int* output_count);
static const char* handle_format_code(const char* c, int* output_count, va_list* args);

static const char* get_format_flags(const char* c, format* cur_format);
static const char* get_format_width(const char* c, format* cur_format, va_list* args);
static const char* get_format_data_size(const char* c, format* cur_format);
static const char* get_format_output(const char* c, format* cur_format, va_list* args);
static int print_format_output(format* cur_format, int* output_count);


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

static const char* handle_color_code(const char* c, int* output_count)
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

static const char* handle_format_code(const char* c, int* output_count, va_list* args)
{
    format cur_format;

    c = get_format_flags(c, &cur_format);
    c = get_format_width(c, &cur_format, args);
    c = get_format_data_size(c, &cur_format);

    char number_string[NUMBER_STRING_LENGTH] = "";
    char prefix_string[4] = "";

    cur_format.output_string_ptr = number_string;
    cur_format.output_prefix_ptr = prefix_string;

    c = get_format_output(c, &cur_format, args);
    print_format_output(&cur_format, output_count);

    return c;
}

static const char* get_format_flags(const char* c, format* cur_format)
{
    // Set default flag variables.
    cur_format->justify_mode = right_justify;
    cur_format->sign_fill_in = '\0';
    cur_format->add_type_prefix = false;

    // Get flags
    while (true) {
        switch (*c) {
            case '-':
                cur_format->justify_mode = left_justify;
                break;
            case '0':
                cur_format->justify_mode = zero_justify;
                break;
            case ' ':
                cur_format->sign_fill_in = ' ';
                break;
            case '+':
                cur_format->sign_fill_in = '+';
                break;
            case '#':
                cur_format->add_type_prefix = true;
                break;
            default:
                // Current char isn't a flag, return.
                return c;
        }
        c++;
    }
}

static const char* get_format_width(const char* c, format* cur_format, va_list* args)
{
    // Set default minimal output width
    cur_format->minimal_output_width = 0;

    // Get minimal output width
    if (*c == '*') {
        cur_format->minimal_output_width = va_arg(*args, int);
        c++;
    } else {
        const char* width_string = c;
        size_t width_string_length = 0;
        while (*c >= '0' && *c <= '9') {
            width_string_length++;
            c++;
        }
        if (width_string_length > 0) {
            cur_format->minimal_output_width = dec_string_to_int(width_string, width_string_length);
        }
    }

    return c;
}

static const char* get_format_data_size(const char* c, format* cur_format)
{
    // Set default data size
    cur_format->data_size = 0;  // Data is int sized (if using integer specifier)

    // Get data size
    if (*c == 'h') {
        c++;
        if (*c == 'h') {
            cur_format->data_size = -2;  // Data is char sized.
            c++;
        } else {
            cur_format->data_size = -1;  // Data is short sized.
        }
    } else if (*c == 'l') {
        c++;
        if (*c == 'l') {
            cur_format->data_size = 2;  // Data is long long sized.
            c++;
        } else {
            cur_format->data_size = 1;  // Data is long sized.
        }
    }

    return c;
}

static const char* get_format_output(const char* c, format* cur_format, va_list* args)
{
    switch (*c) {
        case 's':
            // String
            cur_format->specifier_type = text;

            switch(cur_format->data_size) {
                case 0:
                    cur_format->output_string_ptr = va_arg(*args, char*);
                    break;
                default:
                    goto error;
            }

            break;

        case 'c':
            // Character
            cur_format->specifier_type = text;

            switch(cur_format->data_size) {
                case 0:
                    cur_format->output_string_ptr[0] = va_arg(*args, int);
                    break;
                default:
                    goto error;
            }

            break;

        case 'd':
        case 'i':
            // Signed decimal integer
            cur_format->specifier_type = decimal;

            switch(cur_format->data_size) {
                case -2:
                case -1:
                case 0:
                    cur_format->output_string_ptr = number_to_dec_string(va_arg(*args, int), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 1:
                    cur_format->output_string_ptr = number_to_dec_string(va_arg(*args, long), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                default:
                    goto error;
            }

            if (cur_format->output_string_ptr[0] == '-') {
                cur_format->output_prefix_ptr[0] = '-';
                cur_format->output_string_ptr++;
            } else if (cur_format->sign_fill_in) {
                cur_format->output_prefix_ptr[0] = cur_format->sign_fill_in;
            }
            break;

        case 'u':
            // Unsigned decimal integer
            cur_format->specifier_type = decimal;

            switch(cur_format->data_size) {
                case -2:
                case -1:
                case 0:
                    cur_format->output_string_ptr = unsigned_number_to_dec_string(va_arg(*args, int), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 1:
                    cur_format->output_string_ptr = unsigned_number_to_dec_string(va_arg(*args, long), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                default:
                    goto error;
            }

            if (cur_format->sign_fill_in) {
                cur_format->output_prefix_ptr[0] = cur_format->sign_fill_in;
            }
            break;

        case 'x':
        case 'X':
            // Unsigned heximal integer
            cur_format->specifier_type = heximal;

            switch(cur_format->data_size) {
                case -2:
                    cur_format->output_string_ptr = number_to_hex_string((unsigned char)va_arg(*args, unsigned int), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case -1:
                    cur_format->output_string_ptr = number_to_hex_string((unsigned short)va_arg(*args, unsigned int), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 0:
                    cur_format->output_string_ptr = number_to_hex_string(va_arg(*args, unsigned int), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 1:
                    cur_format->output_string_ptr = number_to_hex_string(va_arg(*args, unsigned long), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                default:
                    goto error;
            }

            if (*c == 'X') {
                if (cur_format->add_type_prefix) {
                    cur_format->output_prefix_ptr[0] = '0';
                    cur_format->output_prefix_ptr[1] = 'X';
                }
                to_uppercase(cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
            } else {
                if (cur_format->add_type_prefix) {
                    cur_format->output_prefix_ptr[0] = '0';
                    cur_format->output_prefix_ptr[1] = 'x';
                }
            }
            break;

        case 'b':
        case 'B':
            // Unsigned binary integer
            cur_format->specifier_type = heximal;

            switch(cur_format->data_size) {
                case -2:
                    cur_format->output_string_ptr = number_to_binary_string((unsigned char)va_arg(*args, unsigned int), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case -1:
                    cur_format->output_string_ptr = number_to_binary_string((unsigned short)va_arg(*args, unsigned int), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 0:
                    cur_format->output_string_ptr = number_to_binary_string(va_arg(*args, unsigned int), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                case 1:
                    cur_format->output_string_ptr = number_to_binary_string(va_arg(*args, unsigned long), cur_format->output_string_ptr, NUMBER_STRING_LENGTH);
                    break;
                default:
                    goto error;
            }

            if (*c == 'B') {
                if (cur_format->add_type_prefix) {
                    cur_format->output_prefix_ptr[0] = '0';
                    cur_format->output_prefix_ptr[1] = 'B';
                }
            } else {
                if (cur_format->add_type_prefix) {
                    cur_format->output_prefix_ptr[0] = '0';
                    cur_format->output_prefix_ptr[1] = 'b';
                }
            }
            break;

        default:
            // Unknown type
            goto error;
    }
    return c;

    error:
    cur_format->output_string_ptr = NULL;
    return c;
}

static int print_format_output(format* cur_format, int* output_count)
{
    if (cur_format->output_string_ptr == NULL)  goto error;

    size_t output_string_length = strlen(cur_format->output_string_ptr) + strlen(cur_format->output_prefix_ptr);

    // Print spaces to justify right if that's the format
    if (cur_format->justify_mode == right_justify) {
        while (output_string_length < cur_format->minimal_output_width) {
            writechar(' ');
            output_string_length++;
        }
    }

    // Print prefix
    writestring(cur_format->output_prefix_ptr);

    // Print zero's to justify if that's the format
    if (cur_format->justify_mode == zero_justify) {
        while (output_string_length < cur_format->minimal_output_width) {
            writechar('0');
            output_string_length++;
        }
    }

    // Print output
    writestring(cur_format->output_string_ptr);

    // Print spaces to justify left if that's the format
    if (cur_format->justify_mode == left_justify) {
        while (output_string_length < cur_format->minimal_output_width) {
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
