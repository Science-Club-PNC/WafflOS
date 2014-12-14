#ifndef _PIC_H
#define _PIC_H

#define PIT_IRQ                  0
#define KEYBOARD_IRQ             1
#define COM2_IRQ                 3
#define COM1_IRQ                 4
#define LPT2_IRQ                 5
#define FLOPPY_IRQ               6
#define LPT1_IRQ                 7
#define CMOS_IRQ                 8
#define PS2_IRQ                 12
#define PRIMARY_ATA_HD_IRQ      14
#define SECONDARY_ATA_HD_IRQ    15

#define PIC1_OFFSET     0x20
#define PIC2_OFFSET     0x28

#define PIT_INTERRUPT                            PIC1_OFFSET + PIT_IRQ
#define KEYBOARD_INTERRUPT                  PIC1_OFFSET + KEYBOARD_IRQ
#define COM2_INTERRUPT                          PIC1_OFFSET + COM2_IRQ 
#define COM1_INTERRUPT                          PIC1_OFFSET + COM1_IRQ
#define LPT2_INTERRUPT                          PIC1_OFFSET + LPT2_IRQ
#define FLOPPY_INTERRUPT                      PIC1_OFFSET + FLOPPY_IRQ
#define LPT1_INTERRUPT                          PIC1_OFFSET + LPT1_IRQ
#define CMOS_INTERRUPT                          PIC2_OFFSET + CMOS_IRQ
#define PS2_INTERRUPT                            PIC2_OFFSET + PS2_IRQ
#define PRIMARY_ATA_HD_INTERRUPT      PIC2_OFFSET + PRIMARY_ATA_HD_IRQ 
#define SECONDARY_ATA_HD_INTERRUPT  PIC2_OFFSET + SECONDARY_ATA_HD_IRQ

#define PIC1		    0x20  // IO base address for master PIC
#define PIC2		    0xA0  // IO base address for slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		    0x20  // End-of-Interrupt command

#define ICW1_ICW4       0x01  // ICW4 (not) needed
#define ICW1_SINGLE     0x02  // Single (cascade) mode
#define ICW1_INTERVAL4  0x04  // Call address interval 4 (8)
#define ICW1_LEVEL      0x08  // Level triggered (edge) mode
#define ICW1_INIT       0x10  // Initialization - required!

#define ICW4_8086       0x01  // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO       0x02  // Auto (normal) EOI
#define ICW4_BUF_SLAVE  0x08  // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C  // Buffered mode/master
#define ICW4_SFNM       0x10  // Special fully nested (not)

void init_PIC();

void PIC_remap(int offset1, int offset2);
void PIC_send_EOI(unsigned char irq);

#endif
