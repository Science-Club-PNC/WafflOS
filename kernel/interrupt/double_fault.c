#include <io.h>

#include "double_fault.h"
#include "../descriptor/idt.h"

extern void double_fault_wrapper();

void double_fault_handler()
{
    printf("Double fault!\n");

    loop:
    goto loop;
}

void add_double_fault_interrupt()
{
    struct idt_entry* entry = &idt[8];
    idt_entry_base(entry, (uint32_t)&double_fault_wrapper);
    entry->attr.pr = 1;
    entry->attr.priv = 0;
    entry->attr.sto = 0;
    entry->attr.type = interrupt_32;
    entry->selector.rpl = 0;
    entry->selector.ti = 0;
    entry->selector.index = 1;
    entry->zero = 0;
}
