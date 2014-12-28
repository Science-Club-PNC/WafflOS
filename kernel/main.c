#include "terminal.h"

void main()
{
    term_clear();
    set_color(green, black);
    writestring("Welcome to WafflOS!");
    update_cursor();
}
