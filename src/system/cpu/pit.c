#include <stdint.h>
#include "io.h"
#include "idt.h"
#include "pit.h"

#define PIT_A 0x40
#define PIT_B 0x41
#define PIT_C 0x42
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SET 0x36
#define PIT_FREQUENCY   1193182

#define TIMER_IRQ 0
#define SUBTICKS_PER_TICK 100

uint64_t ticks = 0;

void
timer_phase(
		int hz
		) {
	int divisor = PIT_FREQUENCY / hz;
	outb(PIT_CONTROL, PIT_SET);
	outb(PIT_A, divisor & PIT_MASK);
	outb(PIT_A, (divisor >> 8) & PIT_MASK);
}

void pit_handler(struct registers_t *regs)
{
    //terminal_print(".");
    ticks++;
    if (ticks % 100 == 0) {
        printf(".");
    }
    pic_sendEOI(0);
}

void pit_init() {
    timer_phase(100);
    register_handler(0x20, pit_handler);
    serial_print("PIT initialized\n");
}

void pit_wait(uint16_t s) {
    uint64_t st = ticks; 
    while (ticks-st < s*100) ;
}