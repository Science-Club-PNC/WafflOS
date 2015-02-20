#ifndef __pic_h__
#define __pic_h__

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28
#define PIC1_COMMAND	0x20  // the command IO port of the master PIC
#define PIC1_DATA	    0x21  // the data IO port of the master PIC
#define PIC2_COMMAND	0xA0  // the command IO port of the slave PIC
#define PIC2_DATA	    0xA1  // the data IO port of the slave PIC

void init_pic();
void pic_remap(int offset1, int offset2);
void pic_clear_mask(unsigned char IRQline);
void pic_set_mask(unsigned char IRQline);
void pic_send_EOI(unsigned char irq);

#endif
