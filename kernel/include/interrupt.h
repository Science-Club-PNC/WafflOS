#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum IDT_gate_type {
    task_gate_32      = 0x5,
    interrupt_gate_16 = 0x6,
    trap_gate_16      = 0x7,
    interrupt_gate_32 = 0xE,
    trap_gate_32      = 0xF
};

struct IDT_type_attr {
    unsigned int gate_type : 4;
    unsigned int for_storage : 1;
    unsigned int privilege: 2;
    unsigned int enabled: 1;
} __attribute__((packed));

struct IDT_descr{
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in LDT
   uint8_t zero;      // unused, set to 0
   struct IDT_type_attr type_attr; // type and attributes, see above
   uint16_t offset_2; // offset bits 16..31
} __attribute__((packed));

inline void set_IDT_descr_offset(struct IDT_descr* descr, uint32_t offset)
{
    descr->offset_1 = (uint16_t)offset;
    descr->offset_2 = (uint16_t)(offset >> 16);
}

inline void set_IDT_descr_selector(struct IDT_descr* descr, uint32_t selector)
{
    descr->selector = selector;
}

inline void set_IDT_descr_gate_type(struct IDT_descr* descr, enum IDT_gate_type gate_type)
{
    descr->type_attr.gate_type = gate_type;
}

inline void set_IDT_descr_for_storage(struct IDT_descr* descr, bool for_storage)
{
    descr->type_attr.for_storage = for_storage;
}

inline void set_IDT_descr_privilege(struct IDT_descr* descr, uint8_t privilege)
{
    descr->type_attr.privilege = privilege;
}

inline void set_IDT_descr_enabled(struct IDT_descr* descr, bool enabled)
{
    descr->type_attr.enabled = enabled;
}

inline uint32_t get_IDT_descr_offset(struct IDT_descr* descr)
{
    return ((uint32_t)descr->offset_1) | ((uint32_t)descr->offset_2 << 16);
}

inline uint32_t get_IDT_descr_selector(struct IDT_descr* descr)
{
    return descr->selector;
}

inline enum IDT_gate_type get_IDT_descr_gate_type(struct IDT_descr* descr)
{
    return descr->type_attr.gate_type;
}

inline bool get_IDT_descr_for_storage(struct IDT_descr* descr)
{
    return descr->type_attr.for_storage;
}

inline uint8_t get_IDT_descr_privilege(struct IDT_descr* descr)
{
    return descr->type_attr.privilege;
}

inline bool get_IDT_descr_enabled(struct IDT_descr* descr)
{
    return descr->type_attr.enabled;
}

void init_IDT();

struct IDT_descr* init_IDT_descr(struct IDT_descr* descr, uint32_t offset, uint16_t selector, enum IDT_gate_type gate_type, bool for_storage, uint8_t privilege, bool enabled);

void set_IDT_descr(size_t interrupt, uint32_t offset, uint16_t selector, enum IDT_gate_type gate_type, bool for_storage, uint8_t privilege, bool enabled);

#endif
