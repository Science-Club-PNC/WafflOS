#ifndef __keyboard_h__
#define __keyboard_h__

#include <stdint.h>
#include <stdbool.h>

enum keycodes{
    keycode_escape = 0x01,
    keycode_1 = 0x02,
    keycode_2 = 0x03,
    keycode_3 = 0x04,
    keycode_4 = 0x05,
    keycode_5 = 0x06,
    keycode_6 = 0x07,
    keycode_7 = 0x08,
    keycode_8 = 0x09,
    keycode_9 = 0x0A,
    keycode_0 = 0x0B,
    keycode_minus = 0x0C,
    keycode_equals = 0x0D,
    keycode_backspace = 0x0E,
    keycode_tab = 0x0F,
    keycode_q = 0x10,
    keycode_w = 0x11,
    keycode_e = 0x12,
    keycode_r = 0x13,
    keycode_t = 0x14,
    keycode_y = 0x15,
    keycode_u = 0x16,
    keycode_i = 0x17,
    keycode_o = 0x18,
    keycode_p = 0x19,
    keycode_bracket_open = 0x1A,
    keycode_bracket_close = 0x1B,
    keycode_enter = 0x1C,
    keycode_left_control = 0x1D,
    keycode_a = 0x1E,
    keycode_s = 0x1F,
    keycode_d = 0x20,
    keycode_f = 0x21,
    keycode_g = 0x22,
    keycode_h = 0x23,
    keycode_j = 0x24,
    keycode_k = 0x25,
    keycode_l = 0x26,
    keycode_semicolon = 0x27,
    keycode_single_quote = 0x28,
    keycode_back_tick = 0x29,
    keycode_left_shift = 0x2A,
    keycode_backslash = 0x2B,
    keycode_z = 0x2C,
    keycode_x = 0x2D,
    keycode_c = 0x2E,
    keycode_v = 0x2F,
    keycode_b = 0x30,
    keycode_n = 0x31,
    keycode_m = 0x32,
    keycode_comma = 0x33,
    keycode_period = 0x34,
    keycode_slash = 0x35,
    keycode_right_shift = 0x36,
    keycode_keypad_asterisk = 0x37,
    keycode_left_alt = 0x38,
    keycode_space = 0x39,
    keycode_caps_lock = 0x3A,
    keycode_F1 = 0x3B,
    keycode_F2 = 0x3C,
    keycode_F3 = 0x3D,
    keycode_F4 = 0x3E,
    keycode_F5 = 0x3F,
    keycode_F6 = 0x40,
    keycode_F7 = 0x41,
    keycode_F8 = 0x42,
    keycode_F9 = 0x43,
    keycode_F10 = 0x44,
    keycode_number_lock = 0x45,
    keycode_scroll_lock = 0x46,
    keycode_keypad_7 = 0x47,
    keycode_keypad_8 = 0x48,
    keycode_keypad_9 = 0x49,
    keycode_keypad_minus = 0x4A,
    keycode_keypad_4 = 0x4B,
    keycode_keypad_5 = 0x4C,
    keycode_keypad_6 = 0x4D,
    keycode_keypad_plus = 0x4E,
    keycode_keypad_1 = 0x4F,
    keycode_keypad_2 = 0x50,
    keycode_keypad_3 = 0x51,
    keycode_keypad_0 = 0x52,
    keycode_keypad_period = 0x53,
    keycode_F11 = 0x57,
    keycode_F12 = 0x58,
};

typedef struct {
    uint8_t press_keycode;
    uint8_t release_keycode;
    char character;
    char upper_character;
    const char* name;
    //const char* upper_name;
} key_data;

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
    };
    uint8_t value;
} keyboard_ctrl_status;

void init_keyboard();
void add_keyboard_interrupt();

const key_data* get_key_data(uint8_t keycode);

keyboard_ctrl_status keyboard_ctrl_get_status();
void keyboard_ctrl_send_cmd(uint8_t cmd);
uint8_t keyboard_encoder_get_buffer();
void keyboard_encoder_send_cmd (uint8_t cmd);

bool keyboard_get_scroll_lock();
void keyboard_set_scroll_lock(bool enabled);
bool keyboard_get_number_lock();
void keyboard_set_number_lock(bool enabled);
bool keyboard_get_caps_lock();
void keyboard_set_caps_lock(bool enabled);

#endif
