#include "interrupt.h"

struct IDT_descr IDT[256];  // The Interrupt Descriptor Table

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

