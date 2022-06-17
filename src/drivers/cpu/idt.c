#include <stdint.h>
#include <stddef.h>

#define GDT_OFFSET_KERNEL_CODE 0x28
#define IDT_MAX_DESCRIPTORS 256

typedef struct {
   uint16_t base_low;        // offset bits 0..15
   uint16_t cs;        // a code segment selector in GDT or LDT
   uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t  attributes; // gate type, dpl, and p fields
   uint16_t base_mid;        // offset bits 16..31
   uint32_t base_high;        // offset bits 32..63
   uint32_t rsv0;            // reserved
} __attribute__((packed)) idt_desc_t;

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) idt_entry_t;

struct registers_t
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t int_no, error_code, rip, cs, rflags, rsp, ss;
} __attribute__((packed));

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtr_t; 

typedef void (*int_handler_t)(struct registers_t*);
int_handler_t interrupt_handlers[256];

__attribute__((aligned(0x10)))
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance
static idtr_t idtr;

void pit_handler(struct registers_t *regs)
{
    terminal_printi(1);
    pic_sendEOI(0);
}

void kbd_handler(struct registers_t *regs)
{
    uint8_t scancode = inb(0x60);
    terminal_printi(scancode);
}

__attribute__((noreturn))
void interrupt_handler(struct registers_t* regs) {
   terminal_print("INTERRUPT: ");
   terminal_printi(regs->int_no);
   if (regs->int_no < 31) __asm__ volatile("cli; hlt");
   else if (interrupt_handlers[regs->int_no]) interrupt_handlers[regs->int_no](regs);
   if (regs->int_no >= 40) outb(0xA0, 0x20);

   outb(0x20, 0x20);
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_desc_t* descriptor = &idt[vector];
 
    descriptor->base_low       = (uint64_t)isr; //& 0xFFFF;
    descriptor->cs             = GDT_OFFSET_KERNEL_CODE;
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->base_mid       = ((uint64_t)isr >> 16);// & 0xFFFF;
    descriptor->base_high      = ((uint64_t)isr >> 32);// & 0xFFFFFFFF;
    descriptor->rsv0           = 0;
}


extern void* isr_stub_table[];
 
void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_desc_t) * IDT_MAX_DESCRIPTORS - 1;

    interrupt_handlers[0x20] = pit_handler;
    interrupt_handlers[0x21] = kbd_handler;
 
    for (size_t i = 0; i < IDT_MAX_DESCRIPTORS; i++) {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E);
        //vectors[vector] = true;
    }

    //pic_unmask(1);
 
    //__asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
}

void enable_interrupt() {
    __asm__ volatile ("sti"); // set the interrupt flag
}

void idt_reload()
{
    __asm__ volatile ("cli");
    __asm__ volatile ("lidt %0" : : "memory"(idtr));
    __asm__ volatile ("sti");
}