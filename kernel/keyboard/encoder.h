#ifndef __encoder_h__
#define __encoder_h__

#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_ENCODER 0x60

uint8_t keyboard_encoder_get_buffer();
uint8_t keyboard_encoder_send_cmd (uint8_t cmd);

void keyboard_set_leds(bool scroll_lock, bool number_lock, bool caps_lock);
void keyboard_set_scan_code_set(uint8_t set);
uint8_t keyboard_get_scan_code_set();
#endif
