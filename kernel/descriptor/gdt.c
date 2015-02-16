#include <stdint.h>
#include <io.h>

#include "load.h"

#include "gdt.h"

#define MAXGDT 5

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;

    struct {
        unsigned int ac: 1;  // Accessed
        unsigned int rw: 1;  // Readable/Writable
        unsigned int dc: 1;  // Direction/Conforming
        unsigned int ex: 1;  // Executable
        unsigned int one: 1;  // Always 1
        unsigned int priv: 2;  // Privilege
        unsigned int pr: 1;  // Present
    } __attribute__((packed)) access;

    union {
        struct {
            unsigned int limit_high: 4;
            unsigned int: 4;  // Padding
        } __attribute__((packed));
        struct {
            unsigned int: 4;  // Padding
            unsigned int zero: 2;  // Always 0
            unsigned int sz: 1;  // Size
            unsigned int gr: 1;  // Granularity
        } __attribute__((packed)) flags;
    };

    uint8_t base_high;
} __attribute__((packed));

void gdt_entry_base(struct gdt_entry* entry, uint32_t base)
{
    entry->base_low = base;
    entry->base_middle = base >> 16;
    entry->base_high = base >> 24;
}

void gdt_entry_limit(struct gdt_entry* entry, uint32_t limit)
{
    if (limit > 0xFFFFF) {
        if ((limit & 0xFFF) != 0xFFF) {
            printf("Unvalid GDT limit, rounding limit up to nearest 0xFFF\n");
        }

        entry->flags.gr = 1;
        limit >>= 12;
    }

    entry->limit_low = limit;
    entry->limit_high = limit >> 16;
}

// Initialize universally standard values for entry
void gdt_entry_init(struct gdt_entry* entry)
{
    entry->access.ac = 0;
    entry->access.pr = 1;
    entry->access.one = 1;
    entry->flags.zero = 0;
}

void load_gdt(struct gdt_entry (*gdt)[], uint16_t size)
{
    struct {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed)) gdtr;

    gdtr.size = size;
    gdtr.base = (uint32_t)gdt;
    lgdt(&gdtr);
}

void init_gdt()
{
    static struct gdt_entry gdt[MAXGDT];

    gdt[1].access.ex = 1;  // Code
    gdt[1].access.priv = 0;  // Kernel

    gdt[2].access.ex = 0;  // Data
    gdt[2].access.priv = 0;  // Kernel

    gdt[3].access.ex = 1;  // Code
    gdt[3].access.priv = 3;  // User

    gdt[4].access.ex = 0;  // Data
    gdt[4].access.priv = 3;  // User

    for (int i = 1; i < MAXGDT; i++) {
        // Set values used in every entry
        gdt[i].access.rw = 1;
        gdt[i].access.dc = 1;
        gdt[i].flags.sz = 1;
        gdt[i].flags.sz = 1;

        gdt_entry_init(&gdt[i]);
        gdt_entry_base(&gdt[i], 0);
        gdt_entry_limit(&gdt[i], 0xFFFFFFFF);
    }

    load_gdt(&gdt, sizeof(gdt));

    __asm__("ljmp %0, $fake_label \n\t fake_label: \n\t" :: "i"(0x8));
}
