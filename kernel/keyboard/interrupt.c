#include <io.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "interrupt.h"
#include "controller.h"
#include "encoder.h"
#include "keys.h"
#include "../descriptor/idt.h"
#include "../hardware/pic.h"

extern void keyboard_wrapper();

void keyboard_handler()
{
    disable_interrupts();

    static int i = 0;
    i++;

    static uint8_t extended_keycode = 0x00;
    static bool break_keycode = false;

    if (keyboard_ctrl_get_status().output_buffer_is_full) {
        uint8_t keycode = keyboard_encoder_get_buffer();
        if (keycode >= 0xE0 && keycode <= 0xE2) {
            extended_keycode = keycode;
        } else if(keycode == 0xF0) {
            break_keycode = true;
        } else {
            printf("keyboard INT #%03i: %s %hhx%hhx\n", i, break_keycode ? "released" : "pressed", extended_keycode, keycode);
            extended_keycode = 0x00;
            break_keycode = false;
        }
    }

    pic_send_EOI(0x1);
    enable_interrupts();
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

    keyboard_disable_translation();
}
