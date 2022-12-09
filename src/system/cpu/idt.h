#ifndef H_IDT
#define H_IDT 1

#define GDT_OFFSET_KERNEL_CODE 0x28
#define IDT_MAX_DESCRIPTORS 256

typedef struct
{
    uint16_t isr_low;   // The lower 16 bits of the ISR's address
    uint16_t kernel_cs; // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t ist;        // The IST in the TSS that the CPU will load into RSP; set to zero for now
    uint8_t attributes; // Type and attributes; see the IDT page
    uint16_t isr_mid;   // The higher 16 bits of the lower 32 bits of the ISR's address
    uint32_t isr_high;  // The higher 32 bits of the ISR's address
    uint32_t reserved;  // Set to zero
} __attribute__((packed)) idt_entry_t;

struct registers_t
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t int_no, error_code, rip, cs, rflags, rsp, ss;
} __attribute__((packed));

typedef struct 
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

typedef void (*int_handler_t)(struct registers_t *);

static uint8_t SYSCALL = 0x69;

void *int_table[];

void idt_init();
void enable_interrupt();
void idt_reload();

void register_handler(uint8_t osffset, int_handler_t handler);

//void _pic_init();

#endif
