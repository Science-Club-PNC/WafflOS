#ifndef __string_h__
#define __string_h__

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

size_t strlen(char* str);
char* to_uppercase(char* str, size_t len);
char* to_lowercase(char* str, size_t len);
char* string_reverse(char* target, size_t len);

char* number_to_dec_string(long a, char* target, size_t len);
char* unsigned_number_to_dec_string(unsigned long a, char* target, size_t len);

char* memory_to_hex_string(const void* memory, size_t memory_size, char* buffer, size_t buffer_len);
#define number_to_hex_string(a, target, len) ({__typeof__(a) memory = a; memory_to_hex_string(&memory, sizeof(memory), target, len);})

char* memory_to_binary_string(const void* memory, size_t memory_size, char* buffer, size_t buffer_len);
#define number_to_binary_string(a, target, len) ({__typeof__(a) memory = a; memory_to_binary_string(&memory, sizeof(memory), target, len);})

int dec_string_to_int(const char* source, size_t len);

#endif
