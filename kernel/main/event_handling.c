#include <base.h>
#include <stddef.h>
#include <io.h>

#include "main.h"
#include "../event.h"
#include "../abort.h"
#include "../keyboard/keys.h"

static void handle_keyboard_event(keyboard_event* event)
{
    if (!event->key_was_released) {
        printf("%c", event->character);
    }
}

void kernel_handle_events()
{
    event_list events = get_events();
    for (size_t event_num = 0; event_num < events.length; event_num++) {
        switch (events.events[event_num].type) {
            case event_type_keyboard:
                handle_keyboard_event((keyboard_event*)&events.events[event_num]);
                break;
            default:
                abort("unkown event found");
        }
    }
}
