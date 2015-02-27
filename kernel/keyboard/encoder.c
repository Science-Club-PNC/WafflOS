#include <stdint.h>
#include <stdbool.h>

#include "encoder.h"
#include "controller.h"
#include "../ioport.h"

uint8_t keyboard_encoder_get_buffer()
{
    return inb(KEYBOARD_ENCODER);
}

uint8_t keyboard_encoder_send_cmd (uint8_t cmd)
{
    while (keyboard_ctrl_get_status().input_buffer_is_full) {
        // Can't send a command just yet.
        // TODO: keyboard may be able to freeze the OS in this part. fix that.
    }

    outb(KEYBOARD_ENCODER, cmd);

    uint8_t response = inb(KEYBOARD_ENCODER);
    if (response == 0xFE) {
        return keyboard_encoder_send_cmd(cmd);
    } else {
        return response;
    }
}

void keyboard_set_leds(bool scroll_lock, bool number_lock, bool caps_lock)
{
    keyboard_encoder_send_cmd(0xED);

    uint8_t output = scroll_lock | (number_lock << 1) | (caps_lock << 2);

    keyboard_encoder_send_cmd(output);
}

void keyboard_set_scan_code_set(uint8_t set)
{
    if (set >= 1 && set <= 3) {
        keyboard_encoder_send_cmd(0xF0);
        keyboard_encoder_send_cmd(set);
    }
}

uint8_t keyboard_get_scan_code_set()
{
    keyboard_encoder_send_cmd(0xF0);
    keyboard_encoder_send_cmd(0);
    return keyboard_encoder_get_buffer();
}
