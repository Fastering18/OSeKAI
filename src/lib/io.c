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

static inline void IoWrite16(uint16_t port, uint16_t data)
{
    __asm__ volatile("outb %b0, %w1" : : "a" (data), "Nd" (port));
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

void write_cr(uint64_t reg, uint64_t val)
{
    switch (reg)
    {
        case 0:
            __asm__ volatile ("mov %0, %%cr0" :: "r" (val) : "memory");
            break;
        case 2:
            __asm__ volatile ("mov %0, %%cr2" :: "r" (val) : "memory");
            break;
        case 3:
            __asm__ volatile ("mov %0, %%cr3" :: "r" (val) : "memory");
            break;
        case 4:
            __asm__ volatile ("mov %0, %%cr4" :: "r" (val) : "memory");
            break;
    }
}

uint64_t read_cr(uint64_t reg)
{
    uint64_t cr;
    switch (reg)
    {
        case 0:
            __asm__ volatile ("mov %%cr0, %0" : "=r" (cr) :: "memory");
            break;
        case 2:
            __asm__ volatile ("mov %%cr2, %0" : "=r" (cr) :: "memory");
            break;
        case 3:
            __asm__ volatile ("mov %%cr3, %0" : "=r" (cr) :: "memory");
            break;
        case 4:
            __asm__ volatile ("mov %%cr4, %0" : "=r" (cr) :: "memory");
            break;
    }
    return cr;
}

// enable SSE
void enableSSE()
{
    write_cr(0, (read_cr(0) & ~(1 << 2)) | (1 << 1));
    write_cr(4, read_cr(4) | (3 << 9));
}

// enable SMEP
void enableSMEP()
{
    write_cr(4, read_cr(4) | (1 << 20));
}

// enable SMAP
void enableSMAP()
{
    write_cr(4, read_cr(4) | (1 << 21));
}