#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <io.h>

#include "keys.h"
#include "encoder.h"

static const key_data keynum_data[] = {
    {'\0', '\0', "unknown", NULL},
    {'\0', '\0', "extend", NULL},

    {'a' , 'A' , "a", "A"},
    {'b' , 'B' , "b", "B"},
    {'c' , 'C' , "c", "C"},
    {'d' , 'D' , "d", "D"},
    {'e' , 'E' , "e", "E"},
    {'f' , 'F' , "f", "F"},
    {'g' , 'G' , "g", "G"},
    {'h' , 'H' , "h", "H"},
    {'i' , 'I' , "i", "I"},
    {'j' , 'J' , "j", "J"},
    {'k' , 'K' , "k", "K"},
    {'l' , 'L' , "l", "L"},
    {'m' , 'M' , "m", "M"},
    {'n' , 'N' , "n", "N"},
    {'o' , 'O' , "o", "O"},
    {'p' , 'P' , "p", "P"},
    {'q' , 'Q' , "q", "Q"},
    {'r' , 'R' , "r", "R"},
    {'s' , 'S' , "s", "S"},
    {'t' , 'T' , "t", "T"},
    {'u' , 'U' , "u", "U"},
    {'v' , 'V' , "v", "V"},
    {'w' , 'W' , "w", "W"},
    {'x' , 'X' , "x", "X"},
    {'y' , 'Y' , "y", "Y"},
    {'z' , 'Z' , "z", "Z"},

    {'1' , '!' , "1", "exclamation mark"},
    {'2' , '@' , "2", "ampersat"},
    {'3' , '#' , "3", "octothorpe"},
    {'4' , '$' , "4", "dollar"},
    {'5' , '%' , "5", "percent"},
    {'6' , '^' , "6", "caret"},
    {'7' , '&' , "7", "ampersand"},
    {'8' , '*' , "8", "asterisk"},
    {'9' , '(' , "9", "open parenthesis"},
    {'0' , ')' , "0", "close parenthesis"},

    {'-' , '_' , "minus", "underscore"},
    {'=' , '+' , "equals", "plus"},
    {'[' , '{' , "open bracket", "open brace"},
    {']' , '}' , "close bracket", "close brace"},
    {'\\', '|' , "backslash", "pipe"},
    {';' , ':' , "semicolon", "colon"},
    {'\'', '"' , "single quote", "quote"},
    {',' , '<' , "comma", "less than"},
    {'.' , '>' , "period", "greater than"},
    {'/' , '?' , "slash", "question mark"},
    {'`' , '~' , "back_tick", "tilde"},

    {' ' , '\0', "space", NULL},
    {'\t', '\0', "tab", NULL},
    {'\n', '\0', "enter", NULL},
    {'\b', '\0', "backspace", NULL},
    {'\0', '\0', "caps_lock", NULL},
    {'\0', '\0', "escape", NULL},

    {'\0', '\0', "F1", NULL},
    {'\0', '\0', "F2", NULL},
    {'\0', '\0', "F3", NULL},
    {'\0', '\0', "F4", NULL},
    {'\0', '\0', "F5", NULL},
    {'\0', '\0', "F6", NULL},
    {'\0', '\0', "F7", NULL},
    {'\0', '\0', "F8", NULL},
    {'\0', '\0', "F9", NULL},
    {'\0', '\0', "F10", NULL},
    {'\0', '\0', "F11", NULL},
    {'\0', '\0', "F12", NULL},

    {'\0', '\0', "print_screen", NULL},
    {'\0', '\0', "scroll_lock", NULL},
    {'\0', '\0', "break", NULL},

    {'\0', '\0', "insert", NULL},
    {'\0', '\0', "delete", NULL},
    {'\0', '\0', "home", NULL},
    {'\0', '\0', "end", NULL},
    {'\0', '\0', "page up", NULL},
    {'\0', '\0', "pag down", NULL},

    {'\0', '\0', "arrow right", NULL},
    {'\0', '\0', "arrow left", NULL},
    {'\0', '\0', "arrow down", NULL},
    {'\0', '\0', "arrow up", NULL},
    {'\0', '\0', "num lock", NULL},

    {'/' , '\0', "keypad divide", NULL},
    {'*' , '\0', "keypad multiply", NULL},
    {'-' , '\0', "keypad minus", NULL},
    {'+' , '\0', "keypad plus", NULL},
    {'\n', '\0', "keypad enter", NULL},
    {'1' , '\0', "keypad 1", NULL},
    {'2' , '\0', "keypad 2", NULL},
    {'3' , '\0', "keypad 3", NULL},
    {'4' , '\0', "keypad 4", NULL},
    {'5' , '\0', "keypad 5", NULL},
    {'6' , '\0', "keypad 6", NULL},
    {'7' , '\0', "keypad 7", NULL},
    {'8' , '\0', "keypad 8", NULL},
    {'9' , '\0', "keypad 9", NULL},
    {'0' , '\0', "keypad 0", NULL},
    {'.' , '\0', "keypad dot", NULL},

    {'\0', '\0', "left shift", NULL},
    {'\0', '\0', "right shift", NULL},
    {'\0', '\0', "left ctrl", NULL},
    {'\0', '\0', "right ctrl", NULL},
    {'\0', '\0', "left alt", NULL},
    {'\0', '\0', "right alt", NULL},
    {'\0', '\0', "left_GUI", NULL},
    {'\0', '\0', "right_GUI", NULL},

    {'\0', '\0', "power", NULL},
    {'\0', '\0', "sleep", NULL},
    {'\0', '\0', "wake", NULL},

    {'\0', '\0', "volume_down", NULL},
    {'\0', '\0', "volume_up", NULL},
    {'\0', '\0', "mute", NULL},

    {'\0', '\0', "email", NULL},
    {'\0', '\0', "apps", NULL},
    {'\0', '\0', "media", NULL},
    {'\0', '\0', "calculator", NULL},
    {'\0', '\0', "my computer", NULL},

    {'\0', '\0', "WWW back", NULL},
    {'\0', '\0', "WWW forward", NULL},
    {'\0', '\0', "WWW stop", NULL},
    {'\0', '\0', "WWW refresh", NULL},
    {'\0', '\0', "WWW home", NULL},
    {'\0', '\0', "WWW favourites", NULL},
    {'\0', '\0', "WWW search", NULL},

    {'\0', '\0', "play/pause", NULL},
    {'\0', '\0', "previous track", NULL},
    {'\0', '\0', "next track", NULL},
    {'\0', '\0', "stop", NULL},
};

static const uint16_t regular_keycode_numbers[] = {
    keynum_unknown,
    keynum_F9,
    keynum_F7,
    keynum_F5,
    keynum_F3,
    keynum_F1,
    keynum_F2,
    keynum_F12,
    keynum_unknown,
    keynum_F10,
    keynum_F8,
    keynum_F6,
    keynum_F4,
    keynum_tab,
    keynum_back_tick,
    keynum_unknown,
    keynum_unknown,
    keynum_left_alt,
    keynum_left_shift,
    keynum_unknown,
    keynum_left_ctrl,
    keynum_q,
    keynum_1,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_z,
    keynum_s,
    keynum_a,
    keynum_w,
    keynum_2,
    keynum_unknown,
    keynum_unknown,
    keynum_c,
    keynum_x,
    keynum_d,
    keynum_e,
    keynum_4,
    keynum_3,
    keynum_unknown,
    keynum_unknown,
    keynum_space,
    keynum_v,
    keynum_f,
    keynum_t,
    keynum_r,
    keynum_5,
    keynum_unknown,
    keynum_unknown,
    keynum_n,
    keynum_b,
    keynum_h,
    keynum_g,
    keynum_y,
    keynum_6,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_m,
    keynum_j,
    keynum_u,
    keynum_7,
    keynum_8,
    keynum_unknown,
    keynum_unknown,
    keynum_comma,
    keynum_k,
    keynum_i,
    keynum_o,
    keynum_0,
    keynum_9,
    keynum_unknown,
    keynum_unknown,
    keynum_period,
    keynum_slash,
    keynum_l,
    keynum_semicolon,
    keynum_p,
    keynum_minus,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_single_quote,
    keynum_unknown,
    keynum_bracket_open,
    keynum_equals,
    keynum_unknown,
    keynum_unknown,
    keynum_caps_lock,
    keynum_right_shift,
    keynum_enter,
    keynum_bracket_close,
    keynum_unknown,
    keynum_backslash,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_backspace,
    keynum_unknown,
    keynum_unknown,
    keynum_keypad_1,
    keynum_unknown,
    keynum_keypad_4,
    keynum_keypad_7,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_keypad_0,
    keynum_keypad_dot,
    keynum_keypad_2,
    keynum_keypad_5,
    keynum_keypad_6,
    keynum_keypad_8,
    keynum_escape,
    keynum_number_lock,
    keynum_F11,
    keynum_keypad_plus,
    keynum_keypad_3,
    keynum_keypad_minus,
    keynum_keypad_multiply,
    keynum_keypad_9,
    keynum_scroll_lock,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_F7,
};
static size_t regular_keycode_numbers_length = sizeof(regular_keycode_numbers) / sizeof(uint16_t);


static const uint16_t alt_keycode_numbers[] = {
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_www_search,
    keynum_right_alt,
    keynum_unknown,
    keynum_unknown,
    keynum_right_ctrl,
    keynum_previous_track,
    keynum_unknown,
    keynum_unknown,
    keynum_www_favourites,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_left_GUI,
    keynum_www_refresh,
    keynum_volume_down,
    keynum_unknown,
    keynum_mute,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_right_GUI,
    keynum_www_stop,
    keynum_unknown,
    keynum_unknown,
    keynum_calculator,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_apps,
    keynum_www_forward,
    keynum_unknown,
    keynum_volume_up,
    keynum_unknown,
    keynum_play_pause,
    keynum_unknown,
    keynum_unknown,
    keynum_power,
    keynum_www_back,
    keynum_unknown,
    keynum_www_home,
    keynum_stop,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_sleep,
    keynum_my_computer,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_email,
    keynum_unknown,
    keynum_keypad_divide,
    keynum_unknown,
    keynum_unknown,
    keynum_next_track,
    keynum_unknown,
    keynum_unknown,
    keynum_media,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_keypad_enter,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_wake,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_end,
    keynum_unknown,
    keynum_arrow_left,
    keynum_home,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_insert,
    keynum_delete,
    keynum_arrow_down,
    keynum_unknown,
    keynum_arrow_right,
    keynum_arrow_up,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_unknown,
    keynum_page_down,
    keynum_unknown,
    keynum_unknown,
    keynum_page_up
};
static size_t alt_keycode_numbers_length = sizeof(alt_keycode_numbers) / sizeof(uint16_t);

static uint8_t key_down_list[KEYNUM_AMOUNT/8] = { 0, 1, 6, 0 };

static bool scroll_lock = false;
static bool number_lock = false;
static bool caps_lock = false;

static bool shift_down = false;
static bool ctrl_down = false;
static bool alt_down = false;

bool keyboard_get_shift_down() {
    return shift_down;
}

bool keyboard_get_ctrl_down() {
    return ctrl_down;
}

bool keyboard_get_alt_down() {
    return alt_down;
}

bool keyboard_get_key_down(uint16_t keynum)
{
    if (keynum < KEYNUM_AMOUNT) {
        return (key_down_list[keynum/8] >> (keynum % 8)) & 1;
    }

    return false;
}

void keyboard_set_key_down(uint16_t keynum, bool key_is_down)
{
    if (keynum < KEYNUM_AMOUNT) {
        if (key_is_down) {
            key_down_list[keynum/8] |= (1 << (keynum % 8));
        } else {
            key_down_list[keynum/8] &= ~(1 << (keynum % 8));
        }

        if (keynum == keynum_left_shift || keynum == keynum_right_shift) {
            shift_down = keyboard_get_key_down(keynum_left_shift) || keyboard_get_key_down(keynum_right_shift);
        } else if (keynum == keynum_left_ctrl || keynum == keynum_right_ctrl) {
            ctrl_down = keyboard_get_key_down(keynum_left_ctrl) || keyboard_get_key_down(keynum_right_ctrl);
        } else if (keynum == keynum_left_alt || keynum == keynum_right_alt) {
            alt_down = keyboard_get_key_down(keynum_left_alt) || keyboard_get_key_down(keynum_right_alt);
        }
    }
}

void handle_keycode(uint8_t extended_keycode, uint8_t keycode, bool break_keycode)
{
    // Get keynum
    uint16_t keynum = keynum_unknown;
    if (extended_keycode == 0) {
        if (keycode < regular_keycode_numbers_length) {
            keynum = regular_keycode_numbers[keycode];
        }
    } else if (extended_keycode == 0xe0) {
        if (keycode < alt_keycode_numbers_length) {
            keynum = alt_keycode_numbers[keycode];
        }
    }

    // Determine event and update key state
    uint8_t event = break_keycode;
    if (break_keycode) {
        keyboard_set_key_down(keynum, false);
    } else {
        if (keyboard_get_key_down(keynum)) {
            event = 2;
        } else {
            keyboard_set_key_down(keynum, true);
        }
    }

    // Switch key locks if any key lock is pressed
    if (event == 0) {
        switch (keynum) {
            case keynum_caps_lock:
                keyboard_set_caps_lock(!caps_lock);
                break;
            case keynum_number_lock:
                keyboard_set_number_lock(!number_lock);
                break;
            case keynum_scroll_lock:
                keyboard_set_scroll_lock(!scroll_lock);
                break;
        }
    }

    // Get key name and character
    const char* key_name;
    char key_character;
    if ((shift_down != caps_lock) && (keynum_data[keynum].upper_name != NULL)) {
        key_name = keynum_data[keynum].upper_name;
        key_character = keynum_data[keynum].upper_character;
    } else {
        key_name = keynum_data[keynum].name;
        key_character = keynum_data[keynum].character;
    }

    static const char* event_names[] = {" pressed", "released", "    held"};
    printf("keyboard: %s %-20s (%c)\t(keycode: %hhx%hhx)\n", event_names[event], key_name, key_character, extended_keycode, keycode);
}

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
