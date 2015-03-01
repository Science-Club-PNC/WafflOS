#ifndef __event_h__
#define __event_h__

#include <stdint.h>

enum event_type{
    event_type_unknown,
    event_type_keyboard,
};

typedef union {
    uint8_t type;
    uint64_t value;
} base_event;

typedef struct {
    const base_event* events;
    size_t length;
} event_list;

typedef struct {
    const uint8_t type;
    union {
        struct {
            unsigned int key_was_released : 1;
            unsigned int key_was_repeated : 1;
            unsigned int shift_down : 1;
            unsigned int ctrl_down : 1;
            unsigned int alt_down : 1;
            unsigned int caps_lock : 1;
            unsigned int number_lock : 1;
            unsigned int scroll_lock : 1;
        } __attribute__((packed));
        unsigned int event : 2;
    } __attribute__((packed));
    uint16_t keynum;
    uint8_t character;
    uint8_t unused1;
    uint16_t unused2;
} __attribute__((packed)) keyboard_event;

void init_events();
event_list get_events();
base_event* add_event(uint8_t type);

#endif
