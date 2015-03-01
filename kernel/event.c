#include <base.h>
#include <stddef.h>
#include <stdint.h>

#include "event.h"
#include "malloc.h"

#define BUFFER_START_LENGTH 0x20
#define BUFFER_GROWTH_STEP 0X10

typedef struct {
    base_event* array;
    size_t length;
    size_t events;
} event_buffer;

static base_event array1[100];
static base_event array2[100];

static event_buffer buffer1 = { array1, 100 , 0};
static event_buffer buffer2 = { array2, 100 , 0};

static event_buffer* front_buffer = &buffer1;
static event_buffer* back_buffer = &buffer2;

void init_events()
{
    // TODO: add dynamic array length
}

event_list get_events() {
    // Clear front buffer
    front_buffer->events = 0;

    // Swap buffers
    event_buffer* temp = back_buffer;
    back_buffer = front_buffer;
    front_buffer = temp;

    event_list list;
    list.events = front_buffer->array;
    list.length = front_buffer->events;
    return list;
}

base_event* add_event(uint8_t type)
{
    if (back_buffer->length <= back_buffer->events) {
        // TODO: add dynamic array length
        return NULL;
    }

    base_event* new_event = &back_buffer->array[back_buffer->events];
    new_event->type = type;
    back_buffer->events++;

    return new_event;
}
