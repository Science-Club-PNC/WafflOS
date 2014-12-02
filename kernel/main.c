#include <string.h>
#include <io.h>
#include "terminal.h"
#include "malloc.h"

void main()
{
	terminal_reset();
    printf("Waffles\n");
    printf("%caffles with %s\n", 'W', "cheese");
    printf("%i waffles in a jar of %i inch\n", 99, 53);
    printf("Wüfflüss\n");
}
