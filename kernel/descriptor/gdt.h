#ifndef __gdt_h__
#define __gdt_h__

typedef struct {
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
} __attribute__((packed)) gdt_entry;

void init_gdt();

#endif
