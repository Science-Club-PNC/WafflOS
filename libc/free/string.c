#include <stddef.h>

#include "string.h"

char* string_reverse(char* target)
{
	size_t len = strlen(target);	
	char* i = target;
	char* j = target + len -1;
	
	while (i < j) {
		char temp = *i;	
		*i = *j;
		*j = temp;
		i++;
		j--;
	}

	return target;
}

char* int_to_string(int a, char* target, size_t len)
{
	size_t i;
	for (i = 0; i < len - 1; i++) {
		target[i] = (char)(a % 10) + '0';
		a /= 10;
		if (a == 0) {
			i++;
			break;
		}
	}	

	target[i] = '\0';

	return string_reverse(target);
}

size_t strlen(char *str)
{
    size_t len = 0;
    while (str[len] != 0) {
        len++;
    }

    return len;
}
