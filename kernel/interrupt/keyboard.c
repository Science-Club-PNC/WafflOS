#include <io.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "keyboard.h"
#include "../ioport.h"
#include "../descriptor/idt.h"
#include "../hardware/pic.h"

#define KEYBOARD_ENCODER 0x60
#define KEYBOARD_CONTROLLER 0x64

extern void keyboard_wrapper();

const key_data key_list1[] = {
    {0, 0, 0, 0, NULL},
    {0x01, 0x01 | 0x80, '\0', '\0', "escape"},
    {0x02, 0x02 | 0x80, '1' , '!' , "1"},
    {0x03, 0x03 | 0x80, '2' , '@' , "2"},
    {0x04, 0x04 | 0x80, '3' , '#' , "3"},
    {0x05, 0x05 | 0x80, '4' , '$' , "4"},
    {0x06, 0x06 | 0x80, '5' , '%' , "5"},
    {0x07, 0x07 | 0x80, '6' , '^' , "6"},
    {0x08, 0x08 | 0x80, '7' , '&' , "7"},
    {0x09, 0x09 | 0x80, '8' , '*' , "8"},
    {0x0A, 0x0A | 0x80, '9' , '(' , "9"},
    {0x0B, 0x0B | 0x80, '0' , ')' , "0"},
    {0x0C, 0x0C | 0x80, '-' , '_' , "minus"},
    {0x0D, 0x0D | 0x80, '=' , '+' , "equals"},
    {0x0E, 0x0E | 0x80, '\b', '\b', "backspace"},
    {0x0F, 0x0F | 0x80, '\t', '\t', "tab"},
    {0x10, 0x10 | 0x80, 'q' , 'Q' , "q"},
    {0x11, 0x11 | 0x80, 'w' , 'W' , "w"},
    {0x12, 0x12 | 0x80, 'e' , 'E' , "e"},
    {0x13, 0x13 | 0x80, 'r' , 'R' , "r"},
    {0x14, 0x14 | 0x80, 't' , 'T' , "t"},
    {0x15, 0x15 | 0x80, 'y' , 'Y' , "y"},
    {0x16, 0x16 | 0x80, 'u' , 'U' , "u"},
    {0x17, 0x17 | 0x80, 'i' , 'I' , "i"},
    {0x18, 0x18 | 0x80, 'o' , 'O' , "o"},
    {0x19, 0x19 | 0x80, 'p' , 'P' , "p"},
    {0x1A, 0x1A | 0x80, '[' , '{' , "bracket_open"},
    {0x1B, 0x1B | 0x80, ']' , '}' , "bracket_close"},
    {0x1C, 0x1C | 0x80, '\n', '\n', "enter"},
    {0x1D, 0x1D | 0x80, '\0', '\0', "left_control"},
    {0x1E, 0x1E | 0x80, 'a' , 'A' , "a"},
    {0x1F, 0x1F | 0x80, 's' , 'S' , "s"},
    {0x20, 0x20 | 0x80, 'd' , 'D' , "d"},
    {0x21, 0x21 | 0x80, 'f' , 'F' , "f"},
    {0x22, 0x22 | 0x80, 'g' , 'G' , "g"},
    {0x23, 0x23 | 0x80, 'h' , 'H' , "h"},
    {0x24, 0x24 | 0x80, 'j' , 'J' , "j"},
    {0x25, 0x25 | 0x80, 'k' , 'K' , "k"},
    {0x26, 0x26 | 0x80, 'l' , 'L' , "l"},
    {0x27, 0x27 | 0x80, ';' , ':' , "semicolon"},
    {0x28, 0x28 | 0x80, '\'', '|' , "single_quote"},
    {0x29, 0x29 | 0x80, '`' , '~' , "back_tick"},
    {0x2A, 0x2A | 0x80, '\0', '\0', "left_shift"},
    {0x2B, 0x2B | 0x80, '\\', '?' , "backslash"},
    {0x2C, 0x2C | 0x80, 'z' , 'Z' , "z"},
    {0x2D, 0x2D | 0x80, 'x' , 'X' , "x"},
    {0x2E, 0x2E | 0x80, 'c' , 'C' , "c"},
    {0x2F, 0x2F | 0x80, 'v' , 'V' , "v"},
    {0x30, 0x30 | 0x80, 'b' , 'B' , "b"},
    {0x31, 0x31 | 0x80, 'n' , 'N' , "n"},
    {0x32, 0x32 | 0x80, 'm' , 'M' , "m"},
    {0x33, 0x33 | 0x80, ',' , '<' , "comma"},
    {0x34, 0x34 | 0x80, '.' , '>' , "period"},
    {0x35, 0x35 | 0x80, '/' , '?' , "slash"},
    {0x36, 0x36 | 0x80, '\0', '\0', "right_shift"},
    {0x37, 0x37 | 0x80, '*' , '*' , "keypad_asterisk"},
    {0x38, 0x38 | 0x80, '\0', '\0', "left_alt"},
    {0x39, 0x39 | 0x80, ' ' , ' ' , "space"},
    {0x3A, 0x3A | 0x80, '\0', '\0', "capslock"},
    {0x3B, 0x3B | 0x80, '\0', '\0', "F1"},
    {0x3C, 0x3C | 0x80, '\0', '\0', "F2"},
    {0x3D, 0x3D | 0x80, '\0', '\0', "F3"},
    {0x3E, 0x3E | 0x80, '\0', '\0', "F4"},
    {0x3F, 0x3F | 0x80, '\0', '\0', "F5"},
    {0x40, 0x40 | 0x80, '\0', '\0', "F6"},
    {0x41, 0x41 | 0x80, '\0', '\0', "F7"},
    {0x42, 0x42 | 0x80, '\0', '\0', "F8"},
    {0x43, 0x43 | 0x80, '\0', '\0', "F9"},
    {0x44, 0x44 | 0x80, '\0', '\0', "F10"},
    {0x45, 0x45 | 0x80, '\0', '\0', "number_lock"},
    {0x46, 0x46 | 0x80, '\0', '\0', "scroll_lock"},
    {0x47, 0x47 | 0x80, '7' , '7' , "keypad_7"},
    {0x48, 0x48 | 0x80, '8' , '8' , "keypad_8"},
    {0x49, 0x49 | 0x80, '9' , '9' , "keypad_9"},
    {0x4A, 0x4A | 0x80, '-' , '-' , "keypad_minus"},
    {0x4B, 0x4B | 0x80, '4' , '4' , "keypad_4"},
    {0x4C, 0x4C | 0x80, '5' , '5' , "keypad_5"},
    {0x4D, 0x4D | 0x80, '6' , '6' , "keypad_6"},
    {0x4E, 0x4E | 0x80, '+' , '+' , "keypad_plus"},
    {0x4F, 0x4F | 0x80, '1' , '1' , "keypad_1"},
    {0x50, 0x50 | 0x80, '2' , '2' , "keypad_2"},
    {0x51, 0x51 | 0x80, '3' , '3' , "keypad_3"},
    {0x52, 0x52 | 0x80, '0' , '0' , "keypad_0"},
    {0x53, 0x53 | 0x80, '.' , '.' , "keypad_period"},
    {0, 0, 0, 0, NULL},
    {0, 0, 0, 0, NULL},
    {0, 0, 0, 0, NULL},
    {0x57, 0x57 | 0x80, '\0', '\0', "F11"},
    {0x58, 0x58 | 0x80, '\0', '\0', "F12"}
};
const size_t key_list1_length = sizeof(key_list1) / sizeof(key_data);

const key_data key_list2[] = {
    {0x00, 0x00 | 0x80, '\0', '\0', "null"},
};
const size_t key_list2_length = sizeof(key_list2) / sizeof(key_data);

void keyboard_handler()
{
    uint8_t scan_keycode = keyboard_encoder_get_buffer();

    const key_data* scan_key_data = get_key_data(scan_keycode);

    if (scan_key_data != NULL) {
        if (scan_key_data->press_keycode == scan_keycode) {
            printf("keyboard: pressed\t| %-20s (%c)\t| (keycode: %#hx)\n", scan_key_data->name, scan_key_data->character, scan_keycode);
        }
        if (scan_key_data->release_keycode == scan_keycode) {
            printf("keyboard: released\t| %-20s (%c)\t| (keycode: %#hx)\n", scan_key_data->name, scan_key_data->character, scan_keycode);
        }

        switch(scan_keycode) {
            case keycode_scroll_lock:
                keyboard_set_scroll_lock(!keyboard_get_scroll_lock());
                break;

            case keycode_number_lock:
                keyboard_set_number_lock(!keyboard_get_number_lock());
                break;

            case keycode_caps_lock:
                keyboard_set_caps_lock(!keyboard_get_caps_lock());
                break;
        }
    } else {
        printf("keyboard: unknown\t| %20s    \t| (keycode: %#hx)\n", "", scan_keycode);
    }

    pic_send_EOI(0x1);
}

void add_keyboard_interrupt()
{
    struct idt_entry* entry = &idt[PIC1_OFFSET + 0x1];
    idt_entry_base(entry, (uint32_t)&keyboard_wrapper);
    entry->attr.pr = 1;
    entry->attr.priv = 0;
    entry->attr.sto = 0;
    entry->attr.type = interrupt_32;
    entry->selector.rpl = 0;
    entry->selector.ti = 0;
    entry->selector.index = 1;
    entry->zero = 0;
}

void init_keyboard()
{
    add_keyboard_interrupt();
    pic_clear_mask(0x1);
}

const key_data* get_key_data(uint8_t keycode)
{
    const key_data* data;
    uint8_t key = keycode & (~0x80);

    if(key <= key_list1_length) {
        data = &key_list1[key];
    } else {
        return NULL;
    }

    if (data == NULL || (data->press_keycode == 0)) {
        return NULL;
    }

    return data;
}

keyboard_ctrl_status keyboard_ctrl_get_status()
{
    keyboard_ctrl_status status;
    status.value = inb(KEYBOARD_CONTROLLER);
    return status;
}

void keyboard_ctrl_send_cmd(uint8_t cmd)
{
    while (keyboard_ctrl_get_status().input_buffer_is_full) {
        // Can't send a command just yet.
        // TODO: keyboard may be able to freeze the OS in this part. fix that.
    }

    outb(KEYBOARD_CONTROLLER, cmd);
}

uint8_t keyboard_encoder_get_buffer()
{
    return inb(KEYBOARD_ENCODER);
}

void keyboard_encoder_send_cmd (uint8_t cmd)
{
    while (keyboard_ctrl_get_status().input_buffer_is_full) {
        // Can't send a command just yet.
        // TODO: keyboard may be able to freeze the OS in this part. fix that.
    }

    outb(KEYBOARD_ENCODER, cmd);
}

static bool scroll_lock = false;
static bool number_lock = false;
static bool caps_lock = false;

void keyboard_update_leds()
{
    keyboard_encoder_send_cmd(0xED);

    uint8_t output = scroll_lock | (number_lock << 1) | (caps_lock << 2);

    keyboard_encoder_send_cmd(output);
}

bool keyboard_get_scroll_lock()
{
    return scroll_lock;
}

void keyboard_set_scroll_lock(bool enabled)
{
    scroll_lock = enabled;
    keyboard_update_leds();
}

bool keyboard_get_number_lock()
{
    return number_lock;
}

void keyboard_set_number_lock(bool enabled)
{
    number_lock = enabled;
    keyboard_update_leds();
}

bool keyboard_get_caps_lock()
{
    return caps_lock;
}

void keyboard_set_caps_lock(bool enabled)
{
    caps_lock = enabled;
    keyboard_update_leds();
}
