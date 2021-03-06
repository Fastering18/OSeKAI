#include "gdt.h"
#include "kernel.h"

__attribute__((aligned(0x1000)))
GDT DefaultGDT =
{
    {0x0000, 0, 0, 0x00, 0x00, 0}, // NULL
    {0xFFFF, 0, 0, 0x9A, 0x00, 0}, // 16 Bit code
    {0xFFFF, 0, 0, 0x92, 0x00, 0}, // 16 Bit data
    {0xFFFF, 0, 0, 0x9A, 0xCF, 0}, // 32 Bit code
    {0xFFFF, 0, 0, 0x92, 0xCF, 0}, // 32 Bit data
    {0x0000, 0, 0, 0x9A, 0x20, 0}, // 64 Bit code
    {0x0000, 0, 0, 0x92, 0x00, 0}, // 64 Bit data
    {0x0000, 0, 0, 0xFA, 0x20, 0}, // User code
    {0x0000, 0, 0, 0xF2, 0x00, 0}, // User data
    {0x0000, 0, 0, 0x89, 0x00, 0, 0, 0} // TSS
};

struct GDTDescriptor gdtDescriptor;
TSS *tss;

void reloadgdt()
{
    __asm__ volatile ("lgdt %0" : : "m"(gdtDescriptor) : "memory");
}

void reloadtss()
{
    __asm__ volatile ("ltr %0" : : "r"((uint16_t)0x48));
}

void reloadall(size_t cpu)
{
    uintptr_t base = (uintptr_t)&tss[cpu];
    uintptr_t limit = base + sizeof(tss[cpu]);

    DefaultGDT.Tss.Length = limit;

    DefaultGDT.Tss.Base0 = base;
    DefaultGDT.Tss.Base1 = base >> 16;
    DefaultGDT.Tss.Flags1 = 0x89;
    DefaultGDT.Tss.Flags2 = 0x00;
    DefaultGDT.Tss.Base2 = base >> 24;
    DefaultGDT.Tss.Base3 = base >> 32;
    DefaultGDT.Tss.Reserved = 0x00;

    reloadgdt();
    reloadtss();
}

void gdt_init()
{
    terminal_print("Initialising GDT");

    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;

    reloadall(0);
    tss[0].RSP[0] = (uint64_t)(kernel_stack + STACK_SIZE);
}

