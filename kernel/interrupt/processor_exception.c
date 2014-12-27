#include <processor_exception.h>
#include <interrupt.h>
#include <io.h>

__attribute__((__noreturn__))
void double_fault_handler()
{
    printf("\n[ABORT]: double fault");
    while (1) { }
    __builtin_unreachable();
}

void init_processor_exceptions() {
    set_IDT_descr(8, (uint32_t)double_fault_handler, 0 /* TODO: don't think fuck it*/, interrupt_gate_32, false, 1, true);
}
