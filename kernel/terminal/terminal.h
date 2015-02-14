#ifndef __terminal_h__
#define __terminal_h__

#include <stddef.h>
#include <stdint.h>

#define TAB_WIDTH 4

typedef enum {
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
} terminal_color;

typedef struct {
    struct {
        unsigned int defined_foreground : 1;
        unsigned int defined_background : 1;
        unsigned int : 6;
    };
    union {
        struct {
            terminal_color fg: 4;
            terminal_color bg: 4;
        };
        uint8_t value;
    };
} terminal_style;

typedef struct {
    char* string;
    size_t allocated_length;
    size_t saving_length;
    size_t display_length;
    terminal_style start_style;
} line_descriptor;

typedef union {
    struct {
        enum {
            style_reset = 1,
            front_color,
            back_color
        } type : 4;
        unsigned int value : 4;
    } __attribute__((packed));
    char c;
} alt_char;

extern size_t line_array_length;
extern line_descriptor* line_array;

void update_view();

void write_char(char c);
void write_string(const char* str);
void put_char(char c);
void put_string(const char* str);

void set_front_color(int color);
void reset_front_color();
void set_back_color(int color);
void reset_back_color();
void reset_style();

int init_line_array(size_t length);
void destroy_line_array();
int resize_line_array(size_t length);

line_descriptor* get_line(size_t line_number);
line_descriptor* forced_get_line(size_t line_number);

int init_line(line_descriptor* line, size_t length);
void destroy_line(line_descriptor* line);
int resize_line(line_descriptor* line, size_t length);

size_t get_line_offset(line_descriptor* line, size_t column);

#endif
