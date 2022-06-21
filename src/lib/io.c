#include <stdint.h>
#include <stdbool.h>
#include "io.h"

 inline uint32_t farpeekl(uint16_t sel, void* off)
{
    uint32_t ret;
    __asm__ ( "push %%fs\n\t"
          "mov  %1, %%fs\n\t"
          "mov  %%fs:(%2), %0\n\t"
          "pop  %%fs"
          : "=r"(ret) : "g"(sel), "r"(off) );
    return ret;
}

inline void farpokeb(uint16_t sel, void* off, uint8_t v)
{
    __asm__ ( "push %%fs\n\t"
          "mov  %0, %%fs\n\t"
          "movb %2, %%fs:(%1)\n\t"
          "pop %%fs"
          : : "g"(sel), "r"(off), "r"(v) );
    /* TODO: Should "memory" be in the clobber list here? */
}

 inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

 inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

 inline void io_wait(void)
{
    outb(0x80, 0);
}

inline bool are_interrupts_enabled()
{
    unsigned long flags;
    __asm__ volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

 inline unsigned long save_irqdisable(void)
{
    unsigned long flags;
    __asm__ volatile ("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
    return flags;
}
 
 inline void irqrestore(unsigned long flags)
{
    __asm__ ("push %0\n\tpopf" : : "rm"(flags) : "memory","cc");
}
 
 void intended_usage(void)
{
    unsigned long f = save_irqdisable();
    irqrestore(f);
}

