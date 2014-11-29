#include <string.h>
#include "terminal.h"

void main()
{
    terminal_reset();
    char amount_of_waffles[3];
    for (int i = 99; i > 0; i--) {
        terminal_setcolor(i % 15, i % 30 / 2);
        terminal_writestring(int_to_string(i, amount_of_waffles, 3));
        terminal_writestring(" waffles of juice on the wall, ");
        terminal_writestring(amount_of_waffles);
        terminal_writestring(" waffles of juice. Take one down, and pass it around, ");
        terminal_writestring(amount_of_waffles);
        terminal_writestring(" waffles of juice on the wall.");
        terminal_writechar('\n');
    }
    terminal_setcolor(color_white, color_black);
    terminal_writestring("No more waffles of juice on the wall, no more waffles of juice. Go to the store, and buy some more. 99 waffles of juice on the wall.");
}
