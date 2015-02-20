#include <base.h>
#include <io.h>

#include "terminal.h"
#include "malloc.h"
#include "descriptor/gdt.h"
#include "descriptor/idt.h"
#include "interrupt/pit.h"
#include "interrupt/keyboard.h"
#include "hardware/pic.h"

void load(char *str)
{
    printf("[$eBUSY$r] %s", str);
}

void ok()
{
    printf("\r[ $aOK$r ] \n");
}

void fail()
{
    printf("\r[$cFAIL$r] \n");
}

void main()
{
    // Just to make sure the PIC doesn't call any undefined interrupts before they're ready.
    disable_interrupts();

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
    init_idt();
    ok();

    load("Initializing PIC");
    init_pic();
    ok();

    load("Initializing PIT");
#define SHUT_UP_PIT  // Disabled because PIT won't shut up xD (temporary)
#ifdef SHUT_UP_PIT
    fail();
#else
    init_pit();
    ok();
#endif

    load("Initializing keyboard");
    init_keyboard();
    ok();

    enable_interrupts();

    printf("\nFinished Initializing, starting tests:\n");

    printf("\nReached end of main(): halting CPU\n");

    loop:
    __asm__("hlt");
    goto loop;
}
