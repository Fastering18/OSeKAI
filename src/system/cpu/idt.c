#include <stdint.h>
#include <stddef.h>
#include "idt.h"
#include "io.h"

int_handler_t interrupt_handlers[256];

__attribute__((aligned(0x10))) static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance
static idtr_t idtr;

static const char *exception_messages[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Detected overflow",
    "Out-of-bounds",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

// interrupt handler for all interrupts
void interrupt_handler(struct registers_t *regs)
{
    //terminal_print("\INT: ");
    //terminal_printi(regs->int_no);
    if (regs->int_no < 32)
    {
        terminal_print("\033[1;91mERROR INTERRUPT: ");
        terminal_printi(regs->int_no);
        terminal_print(" (");
        terminal_print(exception_messages[regs->int_no]);
        terminal_print(")\033[0m\n");

        serial_print("\033[1;91mERROR INTERRUPT: ");
        serial_print(exception_messages[regs->int_no]);
        serial_print("\033[0m\n");

        // hang the system
        __asm__ volatile("cli; hlt");
    }

    else if (interrupt_handlers[regs->int_no])
        interrupt_handlers[regs->int_no](regs);
    if (regs->int_no >= 40)
        outb(0xA0, 0x20);

    outb(0x20, 0x20);
}

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs = GDT_OFFSET_KERNEL_CODE;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}

extern void *int_table[];

void idt_init()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    //interrupt_handlers[0x20] = pit_handler;
    //interrupt_handlers[0x21] = kbd_handler;

    for (size_t i = 0; i < IDT_MAX_DESCRIPTORS; i++)
    {
        idt_set_descriptor(i, int_table[i], 0x8E);
        // vectors[vector] = true;
    }
    idt_set_descriptor(SYSCALL, int_table[SYSCALL], 0xEE);
    // pic_unmask(1);

    //__asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
}

void register_handler(uint8_t osffset, int_handler_t handler) {
    interrupt_handlers[osffset] = handler;
}

void enable_interrupt()
{
    __asm__ volatile("sti"); // set the interrupt flag
}

void idt_reload()
{
    __asm__ volatile("cli");
    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr));
    __asm__ volatile("sti");
}