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
extern void keyboard_wrapper();
void main()
{
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

    printf("keyboard wrapper pointer: %x\n", &keyboard_wrapper);
    printf("main() pointer: %x\n", &main);
    printf("ok() pointer: %x\n", &ok);
    printf("GDT end: %x\n", 0xFFFFF);

    bochs_break();

    __asm__("int $1");

    printf("still alive! :D");

    bochs_break();
}
