#include <gdt.h>

uint64_t GDT[5];

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
 
    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
 
    return descriptor;
}

void init_GDT()
{
    // This basicly disables the use of the GDT.
    GDT[0] = create_descriptor(0, 0, 0); // unused
    GDT[1] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));  // Everything is kernel code.
    GDT[2] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));  // Everything is kernel data.
    GDT[3] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));  // Everything is userspace code.
    GDT[4] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));  // Everything is userspace data.
    // Todo: add TSS descriptor
}
