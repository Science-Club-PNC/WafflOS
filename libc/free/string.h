#ifndef __string_h__
#define __string_h__

#include <stddef.h>

void* memmove(void* dst_ptr, const void* src_ptr, size_t size);
void* memcpy(void* dst_ptr, const void* src_ptr, size_t size);

char *string_reverse(char* target, size_t len);
char *int_to_string(int a, char* target, size_t len);
char *char_to_hex_string(unsigned char a, char* target, size_t len);
size_t strlen(const char* str);

#endif
