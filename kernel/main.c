#include <base.h>
#include <io.h>

#include "terminal.h"
#include "malloc.h"
#include "descriptor/gdt.h"
#include "interrupt/double_fault.h"
#include "interrupt/keyboard.h"
#include "descriptor/idt.h"

void load(char *str) {
    printf("[$eBUSY$r] %s", str);
}

void ok() {
    printf("\r[ $aOK$r ] \n");
}

void fail() {
    printf("\r[$cFAIL$r] \n");
}

void main()
{
    // This just makes reading the bochs debug output a little bit easier. :)
    bochs_print("----------------------------------------\n");
    bochs_print("-------------WafflOS booted-------------\n");
    bochs_print("----------------------------------------\n");

    term_clear();

    printf("\n$!f$0Welcome$R to $!2"OSNAME"$!r version "OSVERSION"!\n\n");

    load("Initializing heap");
    init_heap();
    ok();

    load("Initializing GDT");
    init_gdt();
    ok();

    load("Initializing IDT");
    add_double_fault_idt();
    add_keyboard_idt();
    load_idt();
    ok();

    printf("\nFinished Initializing, starting tests:\n");

    __asm__("int $1");

    printf("\nReached end of main(): halting CPU");
}
