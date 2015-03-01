#include <base.h>
#include <io.h>

#include "pit.h"
#include "../descriptor/idt.h"
#include "../hardware/pic.h"

extern void pit_wrapper();

// TODO: write some PIT control/read functions

static void add_pit_idt()
{
    idt_entry* entry = &idt[PIC1_OFFSET + 0];
    idt_entry_base(entry, (uint32_t)&pit_wrapper);
    entry->attr.pr = 1;
    entry->attr.priv = 0;
    entry->attr.sto = 0;
    entry->attr.type = interrupt_32;
    entry->selector.rpl = 0;
    entry->selector.ti = 0;
    entry->selector.index = 1;
    entry->zero = 0;
}

void pit_handler()
{
    // TODO: Interrupts really shouldn't use printf
    printf("PIT fired\n");
    pic_send_EOI(0x0);
}

void init_pit()
{
    // TODO: Init the pit to not spam the interrupts
    add_pit_idt();
    pic_clear_mask(0);
}
