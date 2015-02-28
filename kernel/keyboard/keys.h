#ifndef __keys_h__
#define __keys_h__

#include <stdint.h>
#include <stdbool.h>

enum keynums {
    keynum_unknown,
    keynum_extend,

    keynum_a,
    keynum_b,
    keynum_c,
    keynum_d,
    keynum_e,
    keynum_f,
    keynum_g,
    keynum_h,
    keynum_i,
    keynum_j,
    keynum_k,
    keynum_l,
    keynum_m,
    keynum_n,
    keynum_o,
    keynum_p,
    keynum_q,
    keynum_r,
    keynum_s,
    keynum_t,
    keynum_u,
    keynum_v,
    keynum_w,
    keynum_x,
    keynum_y,
    keynum_z,

    keynum_1,
    keynum_2,
    keynum_3,
    keynum_4,
    keynum_5,
    keynum_6,
    keynum_7,
    keynum_8,
    keynum_9,
    keynum_0,

    keynum_minus,
    keynum_equals,
    keynum_bracket_open,
    keynum_bracket_close,
    keynum_backslash,
    keynum_semicolon,
    keynum_single_quote,
    keynum_comma,
    keynum_period,
    keynum_slash,
    keynum_back_tick,

    keynum_space,
    keynum_tab,
    keynum_enter,
    keynum_backspace,
    keynum_caps_lock,
    keynum_escape,

    keynum_F1,
    keynum_F2,
    keynum_F3,
    keynum_F4,
    keynum_F5,
    keynum_F6,
    keynum_F7,
    keynum_F8,
    keynum_F9,
    keynum_F10,
    keynum_F11,
    keynum_F12,

    keynum_print_screen,
    keynum_scroll_lock,
    keynum_break,

    keynum_insert,
    keynum_delete,
    keynum_home,
    keynum_end,
    keynum_page_up,
    keynum_page_down,

    keynum_arrow_right,
    keynum_arrow_left,
    keynum_arrow_down,
    keynum_arrow_up,

    keynum_number_lock,
    keynum_keypad_divide,
    keynum_keypad_multiply,
    keynum_keypad_minus,
    keynum_keypad_plus,
    keynum_keypad_enter,
    keynum_keypad_1,
    keynum_keypad_2,
    keynum_keypad_3,
    keynum_keypad_4,
    keynum_keypad_5,
    keynum_keypad_6,
    keynum_keypad_7,
    keynum_keypad_8,
    keynum_keypad_9,
    keynum_keypad_0,
    keynum_keypad_dot,

    keynum_left_shift,
    keynum_right_shift,
    keynum_left_ctrl,
    keynum_right_ctrl,
    keynum_left_alt,
    keynum_right_alt,
    keynum_left_GUI,
    keynum_right_GUI,

    keynum_sleep,
    keynum_wake,

    keynum_volume_down,
    keynum_volume_up,
    keynum_mute,

    keynum_email,
    keynum_apps,
    keynum_media,
    keynum_calculator,
    keynum_my_computer,

    keynum_www_back,
    keynum_www_forward,
    keynum_www_stop,
    keynum_www_refresh,
    keynum_www_home,
    keynum_www_favourites,
    keynum_www_search,

    keynum_play_pause,
    keynum_previous_track,
    keynum_next_track,
    keynum_stop,
};

typedef struct {
    char character;
    char upper_character;
    const char* name;
    const char* upper_name;
} key_data;

void handle_keycode(uint8_t extended_keycode, uint8_t keycode, bool break_keycode);

bool keyboard_get_scroll_lock();
void keyboard_set_scroll_lock(bool enabled);
bool keyboard_get_number_lock();
void keyboard_set_number_lock(bool enabled);
bool keyboard_get_caps_lock();
void keyboard_set_caps_lock(bool enabled);

#endif
