#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "keys.h"
#include "encoder.h"

static bool scroll_lock = false;
static bool number_lock = false;
static bool caps_lock = false;

bool keyboard_get_scroll_lock()
{
    return scroll_lock;
}

void keyboard_set_scroll_lock(bool enabled)
{
    scroll_lock = enabled;
    keyboard_set_leds(scroll_lock, number_lock, caps_lock);
}

bool keyboard_get_number_lock()
{
    return number_lock;
}

void keyboard_set_number_lock(bool enabled)
{
    number_lock = enabled;
    keyboard_set_leds(scroll_lock, number_lock, caps_lock);
}

bool keyboard_get_caps_lock()
{
    return caps_lock;
}

void keyboard_set_caps_lock(bool enabled)
{
    caps_lock = enabled;
    keyboard_set_leds(scroll_lock, number_lock, caps_lock);
}
