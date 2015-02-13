#include <stdint.h>

#include "load.h"

#include "idt.h"

void idt_entry_base(struct idt_entry* entry, uint32_t base)
{
    entry->base_low = base;
    entry->base_high = base >> 16;
}

void load_idt()
{
    struct {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed)) idtr;

    idtr.size = sizeof(idt);
    idtr.base = (uint32_t)idt;
    lidt(&idtr);
}
