#include <interrupt.h>

#define IDT_SIZE 256
struct IDT_descr IDT[IDT_SIZE];  // The Interrupt Descriptor Table

inline void load_IDT(void* base, uint16_t size)
{
    struct
    {
        uint16_t length;
        uint32_t base;
    } __attribute__((packed)) IDTR;

    IDTR.length = size;
    IDTR.base = (uint32_t) base;
    __asm__("lidt (%0)" : : "r"(&IDTR));
}

struct IDT_descr* init_IDT_descr(struct IDT_descr* descr, uint32_t offset, uint16_t selector, enum IDT_gate_type gate_type, bool for_storage, uint8_t privilege, bool enabled)
{
    set_IDT_descr_offset(descr, offset);
    set_IDT_descr_selector(descr, selector);
    set_IDT_descr_gate_type(descr, gate_type);
    set_IDT_descr_for_storage(descr, for_storage);
    set_IDT_descr_privilege(descr, privilege);
    set_IDT_descr_enabled(descr, enabled);

    descr->zero = 0;

    return descr;
}

void set_IDT_descr(size_t interrupt, uint32_t offset, uint16_t selector, enum IDT_gate_type gate_type, bool for_storage, uint8_t privilege, bool enabled)
{
    init_IDT_descr(&IDT[interrupt], offset, selector, gate_type, for_storage, privilege, enabled);
}

void init_IDT()
{
    // Can i assume that IDT is 0 initilized?
    load_IDT(&IDT, IDT_SIZE*sizeof(struct IDT_descr));
}
