#include <base.h>
#include <stdint.h>
#include <io.h>

#include "cpu_exceptions.h"
#include "../abort.h"
#include "../descriptor/idt.h"

extern void divide_by_zero_wrapper();
extern void debug_wrapper();
extern void non_maskable_interrupt_wrapper();
extern void breakpoint_wrapper();
extern void overflow_wrapper();
extern void bound_range_exceeded_wrapper();
extern void invalid_opcode_wrapper();
extern void device_not_available_wrapper();
extern void double_fault_wrapper();
extern void invalid_TSS_wrapper();
extern void segment_not_present_wrapper();
extern void stack_segment_fault_wrapper();
extern void general_protection_fault_wrapper();
extern void page_fault_wrapper();
extern void reserved_wrapper();
extern void x87_floating_point_wrapper();
extern void alignment_check_wrapper();
extern void machine_check_wrapper();
extern void SIMD_floating_point_wrapper();
extern void virtualization_wrapper();

void divide_by_zero_handler()
{
    abort("divide by zero error");
}

void debug_handler()
{
    abort("debug exception");
}

void non_maskable_interrupt_handler()
{
    abort("non maskable interrupt called");
}

void breakpoint_handler()
{
    abort("breakpoint triggered");
}

void overflow_handler()
{
    abort("INTO opcode overflow");
}

void bound_range_exceeded_handler()
{
    abort("BOUND opcode range exceeded");
}

void invalid_opcode_handler()
{
    abort("tried to execute invalid opcode");
}

void device_not_available_handler()
{
    abort("FPU device not available");
}

void double_fault_handler()
{
    abort("double fault occured");
}

void invalid_TSS_handler()
{
    abort("invalid TSS");
}

void segment_not_present_handler()
{
    abort("segment not present");
}

void stack_segment_fault_handler()
{
    abort("stack segment fault");
}

void general_protection_fault_handler()
{
    abort("general protection fault");
}

void page_fault_handler()
{
    abort("page fault");
}

void x87_floating_point_handler()
{
    abort("x87 floating point exception");
}

void alignment_check_handler()
{
    abort("alignment check");
}

void machine_check_handler()
{
    abort("machine check");
}

void SIMD_floating_point_handler()
{
    abort("SIMD floating point exception");
}

void virtualization_handler()
{
    abort("virtualization exception");
}

#define SET_TO_INTERRUPT_32(entry, wrapper) idt_entry_base(entry, (uint32_t)(wrapper));\
    (entry)->attr.pr = 1;\
    (entry)->attr.priv = 0;\
    (entry)->attr.sto = 0;\
    (entry)->attr.type = interrupt_32;\
    (entry)->selector.rpl = 0;\
    (entry)->selector.ti = 0;\
    (entry)->selector.index = 1;\
    (entry)->zero = 0;\

void init_cpu_exceptions()
{
    SET_TO_INTERRUPT_32(idt + 0, divide_by_zero_wrapper);
    SET_TO_INTERRUPT_32(idt + 1, debug_wrapper)
    SET_TO_INTERRUPT_32(idt + 2, non_maskable_interrupt_wrapper)
    SET_TO_INTERRUPT_32(idt + 3, breakpoint_wrapper)
    SET_TO_INTERRUPT_32(idt + 4, overflow_wrapper)
    SET_TO_INTERRUPT_32(idt + 5, bound_range_exceeded_wrapper)
    SET_TO_INTERRUPT_32(idt + 6, invalid_opcode_wrapper)
    SET_TO_INTERRUPT_32(idt + 7, device_not_available_wrapper)
    SET_TO_INTERRUPT_32(idt + 8, double_fault_wrapper)
    SET_TO_INTERRUPT_32(idt + 10, invalid_TSS_wrapper)
    SET_TO_INTERRUPT_32(idt + 11, segment_not_present_wrapper)
    SET_TO_INTERRUPT_32(idt + 12, stack_segment_fault_wrapper)
    SET_TO_INTERRUPT_32(idt + 13, general_protection_fault_wrapper)
    SET_TO_INTERRUPT_32(idt + 14, page_fault_wrapper)
    SET_TO_INTERRUPT_32(idt + 16, x87_floating_point_wrapper)
    SET_TO_INTERRUPT_32(idt + 17, alignment_check_wrapper)
    SET_TO_INTERRUPT_32(idt + 18, machine_check_wrapper)
    SET_TO_INTERRUPT_32(idt + 19, SIMD_floating_point_wrapper)
    SET_TO_INTERRUPT_32(idt + 20, virtualization_wrapper)
}
