#ifndef __controller_h__
#define __controller_h__

#include <stdint.h>

#define KEYBOARD_CONTROLLER 0x64

typedef union {
    struct {
        unsigned int output_buffer_is_full : 1;
        unsigned int input_buffer_is_full : 1;
        unsigned int self_test_succeed : 1;
        unsigned int last_write_was_command : 1;
        unsigned int keyboard_is_locked : 1;
        unsigned int auxiliary_output_is_full : 1;
        unsigned int timeout : 1;
        unsigned int parity_error : 1;
    }__attribute__((packed));
    uint8_t value;
} keyboard_ctrl_status;

keyboard_ctrl_status keyboard_ctrl_get_status();
void keyboard_ctrl_send_cmd(uint8_t cmd);

void keyboard_disable_translation();

#endif
