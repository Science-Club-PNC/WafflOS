#include <base.h>
#include <stdbool.h>

#include "main.h"
#include "../abort.h"

void kernel_main()
{
    kernel_init();

    while (true) {
        // Handle events
        kernel_handle_events();

        // Pause till next interrupt
        __asm__("hlt");
    }

    abort("Reached end of OS code");
}
