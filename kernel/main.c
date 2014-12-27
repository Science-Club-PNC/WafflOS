#include <string.h>
#include <io.h>

#include "terminal.h"
#include "interrupt.h"
#include "malloc.h"
#include "pic.h"
#include "keyboard.h"
#include "gdt.h"
#include "processor_exception.h"

void main()
{
    terminal_reset();
    init_heap();
    init_GDT();
    init_IDT();
    init_PIC();
    init_processor_exceptions();
    init_keyboard();
    
    printf(">> Running \n");
    
    
    /*{
        struct {
            uint16_t length;
            uint32_t base;
        } __attribute__((packed)) IDTR;
        
        IDTR.length = 0xFFFF;
        IDTR.base = 0xFFFFFFFF;
        
        __asm__ volatile ("lidt (%0)" : : "r"(IDTR));
    }
    {
        struct {
            uint16_t length;
            uint32_t base;
        } __attribute__((packed)) IDTR;
    
        IDTR.length = 3;
        IDTR.base = 4;
    
        __asm__ volatile ("sidt (%0)" : : "r"(&IDTR));
    
        printf("length = %i\n",(int) IDTR.length);
        printf("base = %i\n",(int) IDTR.base);
    }*/
    
    printf("%i",3/0); // cause division by zero error.
    
    //__asm__("INT $0x21");  // call keyboard interrupt.
}
