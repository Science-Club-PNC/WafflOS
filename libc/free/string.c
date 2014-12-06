#include <stddef.h>
#include <stdbool.h>

#include "string.h"

char *string_reverse(char *target, size_t len)
{
    char *i = target;
    char *j = target + len - 1;

    while (i < j) {
        char temp = *i;
        *i = *j;
        *j = temp;
        i++;
        j--;
    }

    return target;
}

//TODO: make this function work when a = INT_MIN
char *int_to_string(int a, char *target, size_t len)
{
    bool negative_input = false;
    if (a<0)
    {
        a = -a;
        negative_input = true;
    }
    
    size_t i;
    for (i = 0; i < len - 1; i++) {
        int extract = a % 10;
        target[i] = (char)extract + '0';
        a = (a - extract) / 10;
        if (a == 0) {
            i++;
            break;
        }
    }
    if (negative_input && (i < len - 1)) {
        target[i] = '-';
        i++;
    }
    target[i] = '\0';

    return string_reverse(target, i);
}

char *char_to_hex_string(unsigned char a, char *target, size_t len)
{	
	if (len < 3)
		return NULL;

	char cur_heximal = ((a >> 4) & 0x0F); 
	target[0] = (cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0';
	
	cur_heximal = (a & 0x0F);
	target[1] =  (cur_heximal > 9) ? cur_heximal - 10 + 'A' : cur_heximal + '0';

	target[2] = '\0';

	return target;
}

size_t strlen(char *str)
{
    size_t len = 0;
    while (str[len] != 0) {
        len++;
    }

    return len;
}
