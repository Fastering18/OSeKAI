#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <framebuffer.h>
#include <terminal.h>
#include <alloc.h>
#include <kernel.h>
#include <idt.h>
#include <pic.h>
#include <serial.h>
#include "io.h"

uint8_t *kernel_stack;


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

static void halt(void)
{
    for (;;)
    {
        __asm__("hlt");
    }
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

void
set_fpu_cw(const uint16_t cw) {
	__asm__ volatile("fldcw %0" :: "m"(cw));
}

// kernel's entry point.
void _start(void)
{
    __asm__("movq %%rsp, %0"
            : "=r"(kernel_stack));
    //enableSSE();
    //enableSMEP();
    //enableSMAP();

    // Ensure terminal
    if (terminal_request.response == NULL || terminal_request.response->terminal_count < 1) halt();

    // welcome (serial)
    serial_print("\nWelcome to OSeKAI\n");
    serial_print("github project: https://github.com/Fastering18/OSeKAI\n\n");
    serial_print("======= DEBUG =======\n");

    // initilaize framebuffer & terminal
    framebuffer_init();
    terminal_init();

    terminal_print("Welcome to OSeKAI (build ");
    #ifdef __DATE__ && __TIME__
    terminal_print(__DATE__);
    terminal_print(", ");
    terminal_print(__TIME__);
    #else
    terminal_print("10/12/2022");
    #endif
    terminal_print(")\n");
    terminal_print("Project github.com/Fastering18/OSeKAI\n\n");

    // initialize IDT
    idt_init();
    terminal_print("- IDT initialized\n");

    // initialize pic
    pic_init();
    idt_reload();
    terminal_print("- PIC initialized\n");

    // map memory
    mem_init();
    //terminal_print("- Memory initialized\n");

    //int seperlapan = framebuffer_request.response->framebuffers[0]->height / 8;
    //for (int x = seperlapan; x < framebuffer_request.response->framebuffers[0]->height - seperlapan; x++)
    //    drawhoriline(15, x, 15, torgb(0, 50, 255));

    // debug
    terminal_print("|- CPU count: ");
    terminal_printi(smp_request.response->cpu_count);
    terminal_print("\n");

    terminal_print("|- Memory entries: ");
    terminal_printi(memmap_request.response->entry_count);
    terminal_print("\n");

    terminal_print("|- Boot time at: ");
    terminal_printi(boot_time_request.response->boot_time);
    terminal_print("s (timestamp)\n");

    struct RSDP *rsdp = (struct RSDP *)rsdp_request.response->address;
    terminal_print("|- rsdp at: 0x");
    terminal_printi(rsdp->rsdt_addr);
    terminal_print("\n");

    terminal_print("|- kernel path: ");
    terminal_print(kernel_file_request.response->kernel_file->path);
    terminal_print("\n");

    //terminal_print("\033[6;41mAMOGUS\033[0m\n");
    terminal_print(
        "\n  \033[1;91m_____\033[0m   \033[1;32m_____\033[0m     \033[0;36m__  __\033[0m         \033[1;35m______\033[0m                    \
\n \033[1;91m/ ___ \\\033[0m \033[1;32m/ ____|\033[0m    \033[0;36m| |/ /\033[0m    /\\   \033[1;35m|_   _|\033[0m                          \
\n \033[1;91m| |  | |\033[0m \033[1;32m(___\033[0m   \033[1;93m___\033[0m\033[0;36m| ' /\033[0m    /  \\    \033[1;35m| |\033[0m            \
\n \033[1;91m| |  | |\033[0m\033[1;32m\\___ \\\033[0m \033[1;93m/ _ \\\033[0m  \033[0;36m<\033[0m    / /\\ \\   \033[1;35m| |\033[0m        \
\n \033[1;91m| |__| |\033[0m\033[1;32m____)\033[0m \033[1;93m|  __/\033[0m \033[0;36m. \\\033[0m  / ____ \\ \033[1;35m_| |_\033[0m          \
\n \033[1;91m\\____|\033[0m\033[1;32m_______/\033[0m \033[1;93m\\___|\033[0m\033[0;36m_|\\_\\\033[0m/_/    \\_\\\033[1;35m_____|\033[0m     \n\
  ");
    terminal_print("\033[1;37mOperasi Sistem e Karya Anak Indonesia\033[0m\n");

    size_t cr4;
	__asm__ volatile ("mov %%cr4, %0" : "=r"(cr4));
	cr4 |= 0x200;
	__asm__ volatile ("mov %0, %%cr4" :: "r"(cr4));
	set_fpu_cw(0x37F);

    /*for (size_t i = 0; i < module_request.response->module_count; i++) {
        terminal_print(module_request.response->modules[i]->cmdline);
        terminal_print("\n");
    }*/

    // IDT interrupt test
    //__asm__("int $0x10");

    // halt
    halt();
}