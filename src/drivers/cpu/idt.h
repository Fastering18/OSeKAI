#ifndef H_IDT
#define H_IDT 1

#define GDT_OFFSET_KERNEL_CODE 0x28
#define IDT_MAX_DESCRIPTORS 256

static uint8_t SYSCALL = 0x69;

void *int_table[];

void idt_init();
void enable_interrupt();
void idt_reload();

#endif
