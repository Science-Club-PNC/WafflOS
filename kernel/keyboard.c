#include "keyboard.h"
#include "interrupt.h"
#include "pic.h"

#include <io.h>

extern uint32_t keyboard_wrapper;

void keyboard_handler()
{
    printf("KEY PRESS!!! ");
}

void init_keyboard()
{
    set_IDT_descr(KEYBOARD_INTERRUPT, keyboard_wrapper, 0 /* TODO: don't think fuck it*/, interrupt_gate_32, false, 1, true);
}

