#include <io.h>
#include "terminal.h"

#define OSNAME "WafflOS"

void load(char *str) {
    printf("        %s", str);
}

void ok() {
    printf("\r [ &2OK&r ]\n");
}

void fail() {
    printf("\r [&4FAIL&r]\n");
}

void main()
{
    term_clear();
    printf("\n$e&4Welcome%r to &2%s&r!\n\n", OSNAME);
    load("Loading whatever");
    // Do stuff
    ok();
    load("Loading something destined to fail miserably");
    // Do some error stuff
    fail();
}
