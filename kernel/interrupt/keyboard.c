#include <io.h>

#include "keyboard.h"
#include "../ioport.h"
#include "../descriptor/idt.h"
#include "../hardware/pic.h"

extern void keyboard_wrapper();

void keyboard_handler()
{
    // TODO: actually handle these keyboard interrupts
    // TODO: find some documentation about what this IO's do...
    uint8_t scan = inb(0x60);
    uint8_t i = inb(0x61);
    outb(0x61, i|0x80);
    outb(0x61, i);

    // TODO: Interrupts really shouldn't use printf
    printf("Key pressed! scan: %#hx, i: %#hx\n", scan, i);

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
