#include <base.h>
#include <io.h>

#include "terminal/terminal.h"
#include "terminal/vga.h"
#include "malloc.h"

void load(char *str) {
    printf("\r[$eBUSY$r] %s", str);
}

void ok() {
    printf("\r[ $aOK$r ] \n");
}

void fail() {
    printf("\r[$cFAIL$r] \n");
}

void main()
{
    vga_clear();
    init_heap();
    init_line_array(0);

    load("loading terminator");
    printf("\n");
    load("loading something terribly long... yolololololololololololololololololololololololololololololololololololo");
    fail();

    update_view();
}
