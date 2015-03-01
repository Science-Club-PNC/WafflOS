#include <base.h>
#include <io.h>

#include "main.h"
#include "../abort.h"
#include "../terminal.h"
#include "../malloc.h"
#include "../event.h"
#include "../descriptor/gdt.h"
#include "../descriptor/idt.h"
#include "../interrupt/pit.h"
#include "../keyboard/interrupt.h"
#include "../hardware/pic.h"

static void load(char *str) {
    printf("\r[$eBUSY$r] %s", str);
}

static void ok()
{
    printf("\r[ $aOK$r ] \n");
}

static void fail()
{
    printf("\r[$cFAIL$r] \n");
}

void kernel_init()
{
    // Just to make sure the PIC doesn't call any undefined interrupts before they're ready.
    disable_interrupts();

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
    fail();

    load("Initializing keyboard");
    init_keyboard();
    ok();

    enable_interrupts();

    printf("\nFinished Initializing, starting main loop:\n");
}
