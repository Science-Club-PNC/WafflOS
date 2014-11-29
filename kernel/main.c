#include <string.h>
#include "terminal.h"

void main()
{
    terminal_reset();
    for (int i = 0; i < 10; i++) {
        terminal_setcolor(i % 15, i % 30 / 2);
        char number[5] = { 0 };
        terminal_writestring(int_to_string(i, number, 5));
        terminal_writestring(" lines: ");
        for (int x = 0; x < i; x++) {
            terminal_writechar('|');
        }
        terminal_writechar('\n');
    }
}
