#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <framebuffer.h>
#include <terminal.h>
#include <alloc.h>
#include <kernel.h>
#include <idt.h>
#include <pic.h>
#include "io.h"

uint8_t *kernel_stack;

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.
#if LVL5_PAGING
volatile struct limine_5_level_paging_request _5_level_paging_request =
    {
        .id = LIMINE_5_LEVEL_PAGING_REQUEST,
        .revision = 0,
        .response = NULL};
#endif

volatile struct limine_terminal_request terminal_request =
    {
        .id = LIMINE_TERMINAL_REQUEST,
        .revision = 0,
        .response = NULL,
        .callback = terminal_callback};

volatile struct limine_framebuffer_request framebuffer_request =
    {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0,
        .response = NULL};

volatile struct limine_smp_request smp_request =
    {
        .id = LIMINE_SMP_REQUEST,
        .revision = 0,
        .response = NULL,
        .flags = (1 << 0)};

volatile struct limine_memmap_request memmap_request =
    {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 0,
        .response = NULL};

volatile struct limine_rsdp_request rsdp_request =
    {
        .id = LIMINE_RSDP_REQUEST,
        .revision = 0,
        .response = NULL};

volatile struct limine_module_request module_request =
    {
        .id = LIMINE_MODULE_REQUEST,
        .revision = 0,
        .response = NULL};

volatile struct limine_kernel_file_request kernel_file_request =
    {
        .id = LIMINE_KERNEL_FILE_REQUEST,
        .revision = 0,
        .response = NULL};

volatile struct limine_boot_time_request boot_time_request =
    {
        .id = LIMINE_BOOT_TIME_REQUEST,
        .revision = 0,
        .response = NULL};

volatile struct limine_hhdm_request hhdm_request =
    {
        .id = LIMINE_HHDM_REQUEST,
        .revision = 0,
        .response = NULL};

volatile struct limine_kernel_address_request kernel_address_request =
    {
        .id = LIMINE_KERNEL_ADDRESS_REQUEST,
        .revision = 0,
        .response = NULL};

volatile struct limine_stack_size_request stack_size_request =
    {
        .id = LIMINE_STACK_SIZE_REQUEST,
        .revision = 0,
        .response = NULL,
        .stack_size = STACK_SIZE};

static void done(void)
{
    for (;;)
    {
        __asm__("hlt");
    }
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

struct RSDP
{
    uint8_t signature[8];
    uint8_t checksum;
    uint8_t oem_id[6];
    uint8_t revision;
    uint32_t rsdt_addr;
    uint32_t length;
    uint64_t xsdt_addr;
    uint8_t ext_checksum;
    uint8_t reserved[3];
};

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

void enableSSE()
{
    write_cr(0, (read_cr(0) & ~(1 << 2)) | (1 << 1));
    write_cr(4, read_cr(4) | (3 << 9));
}

// kernel's entry point.
void _start(void)
{
    __asm__("movq %%rsp, %0"
            : "=r"(kernel_stack));
    enableSSE();

    // Ensure we got a terminal
    if (terminal_request.response == NULL || terminal_request.response->terminal_count < 1) done();

    framebuffer_init();
    terminal_init();

    idt_init();
    pic_init();
    idt_reload();

    // pic_unmask(0);
    // pic_unmask(12);
    mem_init();
    

    int seperlapan = framebuffer_request.response->framebuffers[0]->height / 8;
    for (int x = seperlapan; x < framebuffer_request.response->framebuffers[0]->height - seperlapan; x++)
        drawhoriline(15, x, 15, torgb(0, 50, 255));

    // prints
    terminal_print("\nCPU count: ");
    terminal_printi(smp_request.response->cpu_count);
    terminal_print("\nMemory entries: ");
    terminal_printi(memmap_request.response->entry_count);
    terminal_print("\nBoot time at: ");
    terminal_printi(boot_time_request.response->boot_time);
    terminal_print("s (timestamp)");

    // terminal_printi(module_request.response->module_count);
    // terminal_printi(smp_request.response->cpus[0]->processor_id);

    // terminal_print("\nFree pages: ");
    // terminal_printi(FREE_PAGES);
    struct RSDP *rsdp = (struct RSDP *)rsdp_request.response->address;
    terminal_print("\nrsdp at: ");
    terminal_printi((int)rsdp);
    terminal_print("\nrev: ");
    terminal_printi(rsdp->revision);
    terminal_print("\n\033[6;41mAMOGUS\033[0m");
    // terminal_print("\033[3A");
    terminal_print(
        "\n  \033[1;91m_____\033[0m   \033[1;32m_____\033[0m     \033[0;36m__  __\033[0m         \033[1;35m______\033[0m                    \
\n \033[1;91m/ ___ \\\033[0m \033[1;32m/ ____|\033[0m    \033[0;36m| |/ /\033[0m    /\\   \033[1;35m|_   _|\033[0m                   \
\n \033[1;91m| |  | |\033[0m \033[1;32m(___\033[0m   \033[1;93m___\033[0m\033[0;36m| ' /\033[0m    /  \\    \033[1;35m| |\033[0m                     \
\n \033[1;91m| |  | |\033[0m\033[1;32m\\___ \\\033[0m \033[1;93m/ _ \\\033[0m  \033[0;36m<\033[0m    / /\\ \\   \033[1;35m| |\033[0m                     \
\n \033[1;91m| |__| |\033[0m\033[1;32m____)\033[0m \033[1;93m|  __/\033[0m \033[0;36m. \\\033[0m  / ____ \\ \033[1;35m_| |_\033[0m                     \
\n \033[1;91m\\____|\033[0m\033[1;32m_______/\033[0m \033[1;93m\\___|\033[0m\033[0;36m_|\\_\\\033[0m/_/    \\_\\\033[1;35m_____|\033[0m                   \
  ");
    terminal_print("\n\033[1;37mOperasi Sistem e Karya Anak Indonesia\033[0m\n");

    // IDT interrupt test
    __asm__("int $0x10");

    // hang...
    done();
}