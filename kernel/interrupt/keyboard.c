#include <io.h>

#include "keyboard.h"
#include "../descriptor/idt.h"

extern void keyboard_wrapper();

void keyboard_handler() {
    printf("Key pressed!\n");
}

void add_keyboard_idt() {
    struct idt_entry* entry = &idt[1];
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
