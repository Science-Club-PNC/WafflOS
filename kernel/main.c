#include <io.h>
#include "terminal.h"

#define OSNAME "WafflOS"

void main()
{
    term_clear();
    printf("$e&4Welcome%r to &2%s&r!", OSNAME);
}
