#include <base.h>
#include <stdint.h>

#include "ioport.h"

void outb(uint16_t port, uint8_t val)
{
    __asm__("outb %0, %1" : : "a"(val), "Nd"(port));
}

void outw(uint16_t port, uint16_t val)
{
    __asm__("outw %0, %1" : : "a"(val), "Nd"(port));
}

void outl(uint16_t port, uint32_t val)
{
    __asm__("outl %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm__( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

uint32_t inl(uint16_t port)
{
    uint32_t ret;
    __asm__( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

void io_wait()
{
    // TODO: maybe find a more sleak/trusty way to do this?
    __asm__ volatile( "jmp 1f\n\t"
                      "1: jmp 2f\n\t"
                      "2: " );
}
