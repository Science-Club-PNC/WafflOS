#include <base.h>
#include <stdint.h>

#include "pic.h"
#include "../ioport.h"


#define PIC_EOI                 0x20  // End Of Interrupt command code
#define PIC_READ_IRR            0x0a  // read Interrupt Request Register command code
#define PIC_READ_ISR            0x0b  // read In-Service Register command code

// PIC init flags for ICW1
#define PIC_ICW1_ICW4           0x01
#define PIC_ICW1_SINGLE         0x02
#define PIC_ICW1_INTERVAL4      0x04
#define PIC_ICW1_LEVEL          0x08
#define PIC_ICW1_INIT           0x10

// PIC init flags for ICW4
#define PIC_ICW4_8086           0x01
#define PIC_ICW4_AUTO           0x02
#define PIC_ICW4_BUF_SLAVE      0x08
#define PIC_ICW4_BUF_MASTER     0x0C
#define PIC_ICW4_SFNM           0x10

static void pic_remap(uint8_t offset1, uint8_t offset2)
{
    // Init PICs in single (cascade) mode
    outb(PIC1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();

    // Give PICs new Interrupt offsets
    outb(PIC1_DATA, offset1);
    io_wait();
    outb(PIC2_DATA, offset2);
    io_wait();

    // Give PICs magic numbers... ?
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    // Set PICs to 8086 mode
    outb(PIC1_DATA, PIC_ICW4_8086);
    io_wait();
    outb(PIC2_DATA, PIC_ICW4_8086);
    io_wait();

    // Reset masks (thus disable all IRQs)
    outb(PIC1_DATA, 0xF);
    outb(PIC2_DATA, 0xF);
}

void init_pic()
{
    pic_remap(PIC1_OFFSET, PIC2_OFFSET);
}

void pic_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);
}

void pic_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}

void pic_send_EOI(uint8_t irq)
{
        if(irq >= 8) {
                outb(PIC2_COMMAND,PIC_EOI);
    }
        outb(PIC1_COMMAND,PIC_EOI);
}
