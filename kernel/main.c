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

    printf("this is a very long string because of various reasons. primarly testing tho\nNEWLINE!!!\ni hate cake\ri like\nnewline\n123456789A 123456789B 123456789C 123456789D 123456789E 123456789A 123456789B 123456789C 123456789D 123456789E\n\n");
    write_string("ab\rc\n");
    update_view();
}
