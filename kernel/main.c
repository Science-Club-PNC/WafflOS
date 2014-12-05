#include <string.h>
#include <io.h>

#include "terminal.h"
#include "malloc.h"

void main()
{
	terminal_reset();
    printf("Waffles\n");
    printf("%caffles with %s and %s\n", 'W', "cheese", "spaghetti");
    printf("%i waffles in a jar of %i inch\n", 99, 53);
    printf("Wüfflüss");
}
