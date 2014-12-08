#include <string.h>
#include <io.h>

#include "terminal.h"
#include "malloc.h"

void main()
{
    terminal_reset();

    init_heap();
    test_heap();
}
