#include <string.h>
#include <io.h>

#include "terminal.h"
#include "interrupt.h"
#include "malloc.h"
#include "pic.h"
#include "keyboard.h"
#include "gdt.h"

void main()
{
    terminal_reset();
    init_heap();
    init_GDT();
    //init_IDT();
    //init_PIC();
    //init_keyboard();
    printf("> Running \n");
    //__asm__("INT $0x21");
}
