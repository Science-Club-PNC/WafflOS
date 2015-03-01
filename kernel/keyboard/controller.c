#include <base.h>
#include <stdint.h>

#include "controller.h"
#include "encoder.h"
#include "../ioport.h"

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

void keyboard_disable_translation()
{
    // Get command byte
    keyboard_ctrl_send_cmd(0x20);
    uint8_t cmd =  keyboard_encoder_get_buffer();

    // Clear Translation bit
    cmd &= ~0x40;

    // Set command byte
    keyboard_ctrl_send_cmd(0x60);
    keyboard_encoder_send_cmd(cmd);
}
