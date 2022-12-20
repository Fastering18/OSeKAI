#include <stdint.h>
#include <stddef.h>
#include "io.h"
#include "idt.h"
#include "mem.h"
#include "kbd.h"

unsigned char toUpperCase(unsigned char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 32;
    return c;
}

void kbd_ack(void)
{
    while (!(inb(0x60) == 0xfa))
        ;
}

void kbd_led_handling(char ledstatus)
{
    outb(0x60, 0xed);
    kbd_ack();
    outb(0x60, ledstatus);
}

// US keyboard scancodes to ascii
char sc2ascii[] = {SCANCODE2ASCII_TABLE};

static int kbd_caps_lock = 0;
static int kbd_shift = 0;

static char captureln[100];

void kbd_handler(struct registers_t *regs)
{
    uint8_t scancode = inb(0x60);
    unsigned char *ascii = {sc2ascii[scancode], '\0'};

    if (scancode == 42 || scancode == 54) {
        kbd_shift = 1;
    } else if (scancode == 170 || scancode == 182) {
        kbd_shift = 0;
    }

    if ((kbd_caps_lock && !kbd_shift) || (!kbd_caps_lock && kbd_shift)) { 
        ascii = toUpperCase(ascii);
    }
    //terminal_print("sc: ");
    //terminal_printi(scancode);

    switch (scancode)
    {
    case 0x3A:
        kbd_caps_lock = !kbd_caps_lock;
        //kbd_led_handling(4);
        break;
    default:
        terminal_print(&ascii);
        char ca[2] = {sc2ascii[scancode], '\0'};
        strcat(captureln, ca);
        
        if (ascii == 13)
        {
            if (strncmp(captureln, "turu0x10", 8) == 0) {
                __asm__ ("int $0x10");
            }

            terminal_print("\n");
            memset(captureln, 0, 100 * sizeof(char));
        }
        break;
    }

    pic_sendEOI(1);
}

void kbd_init() {
    register_handler(0x21, kbd_handler);
    serial_print("PS/2 Keyboard initialized\n");
}