#ifndef __terminal_h__
#define __terminal_h__

#include <stddef.h>

#define ALT_MEANING_CHAR '\0'
#define TAB_WIDTH 4

typedef struct {
    char* string;
    size_t allocated_length;
    size_t saving_length;
    size_t display_length;
} line_descriptor;

extern char debug_string[25];

extern size_t line_array_length;
extern line_descriptor* line_array;

void update_view();

void write_char(char c);
void write_string(const char* str);
void put_char(char c);
void put_string(const char* str);

void test_put_char(char c);

int init_line_array(size_t length);
void destroy_line_array();
int resize_line_array(size_t length);

line_descriptor* get_line(size_t line_number);
line_descriptor* forced_get_line(size_t line_number);

int init_line(line_descriptor* line, size_t length);
void destroy_line(line_descriptor* line);
int resize_line(line_descriptor* line, size_t length);

#endif
