#include <stdint.h>
#include <kernel.h>
#include "io.h"

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)
#define PIC_EOI 0x20 /* End-of-interrupt command code */

#define ICW1_ICW4 0x01      /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08     /* Level triggered (edge) mode */
#define ICW1_INIT 0x10      /* Initialization - required! */

#define ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02       /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10       /* Special fully nested (not) */


// pic send eoi
void pic_sendEOI(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

// disable pic
void pic_disable()
{
    outb(PIC1_COMMAND, 0xFF);
    outb(PIC2_COMMAND, 0xFF);
}

/**
 * @brief PIC initialization function
 * pic_init() initializes the PIC and sets up the IRQs.
 * pic_init() is called by the kernel during initialization.
 * @return void
 */
void pic_init() {
    // ICW1: Master PIC
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // ICW2: Master PIC
    outb(PIC1_DATA, 0x20);
    io_wait();

    // ICW3: Master PIC
    outb(PIC1_DATA, 4);
    io_wait();

    // ICW4: Master PIC
    outb(PIC1_DATA, ICW4_8086);
    io_wait();

    // ICW1: Slave PIC
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // ICW2: Slave PIC
    outb(PIC2_DATA, 0x28);
    io_wait();

    // ICW3: Slave PIC
    outb(PIC2_DATA, 2);
    io_wait();

    // ICW4: Slave PIC
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Mask all IRQs
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);

    // set up the PIT
    outb(0x43, 0x34);
    outb(0x40, 0x00);
    outb(0x40, 0x00);
    outb(0x40, 0x00);

    // set up the keyboard
    outb(0x21, 0xFD);
    outb(0xA1, 0xFD);

    // set up mouse
    outb(0xA9, 0xFF);
}


void pic_mask(unsigned char IRQline)
{
    uint16_t port;
    uint8_t value;

    if (IRQline < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);
}

void pic_unmask(unsigned char IRQline)
{
    uint16_t port;
    uint8_t value;

    if (IRQline < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}