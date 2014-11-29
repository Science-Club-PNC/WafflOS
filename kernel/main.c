#include <string.h>
#include "terminal.h"

void main()
{
    terminal_reset();
    for (int i = 0; i < 40; i++) {
        terminal_setcolor(i % 15, i % 30 / 2);
        terminal_writestring("Waffles ");
        for (int x = 0; x < i; x++) {
            terminal_writechar('|');
        }
        terminal_writechar('\n');
    }
}
