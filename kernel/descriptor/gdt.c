#include <base.h>
#include <stdint.h>
#include <io.h>

#include "load.h"

#include "gdt.h"

#define MAXGDT 5

static void gdt_entry_base(gdt_entry* entry, uint32_t base)
{
    entry->base_low = base;
    entry->base_middle = base >> 16;
    entry->base_high = base >> 24;
}

static void gdt_entry_limit(gdt_entry* entry, uint32_t limit)
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
static void gdt_entry_init(gdt_entry* entry)
{
    entry->access.ac = 0;
    entry->access.pr = 1;
    entry->access.one = 1;
    entry->flags.zero = 0;
}

static void load_gdt(gdt_entry (*gdt)[], uint16_t size)
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
    static gdt_entry gdt[MAXGDT];

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
