#ifndef __idt_h__
#define __idt_h__

#include <stdint.h>

#define MAXIDT 256

#define enable_interrupts() __asm__("sti")
#define disable_interrupts() __asm__("cli")

struct idt_entry {
    uint16_t base_low;

    struct {
        unsigned int rpl: 2;
        unsigned int ti: 1;
        unsigned int index: 13;
    } __attribute__((packed)) selector;

    uint8_t zero;

    struct {
        enum {
            task = 5,
            interrupt_16 = 6,
            trap_16 = 7,
            interrupt_32 = 14,
            trap_32 = 15
        } type: 4;  // Gate Type
        unsigned int sto: 1;  // Storage Segment
        unsigned int priv: 2;  // Privilege
        unsigned int pr: 1;  // Present
    } __attribute__((packed)) attr;

    uint16_t base_high;
} __attribute__((packed));

struct idt_entry idt[MAXIDT];

void idt_entry_base(struct idt_entry* entry, uint32_t base);
void load_idt();
void init_idt();

#endif
