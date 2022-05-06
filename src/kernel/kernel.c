#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <framebuffer.h>
#include <terminal.h>
#include <kernel.h>

uint8_t *kernel_stack;

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.
#if LVL5_PAGING
volatile struct limine_5_level_paging_request _5_level_paging_request =
{
    .id = LIMINE_5_LEVEL_PAGING_REQUEST,
    .revision = 0,
    .response = NULL
};
#endif

volatile struct limine_terminal_request terminal_request = 
{
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0,
    .response = NULL,
    .callback = terminal_callback
};

volatile struct limine_framebuffer_request framebuffer_request =
{
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = NULL
};

volatile struct limine_smp_request smp_request =
{
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
    .response = NULL,
    .flags = (1 << 0)
};

static void done(void) {
    for (;;) {
        __asm__("hlt");
    }
}

// The following will be our kernel's entry point.
void _start(void) {
    __asm__("movq %%rsp, %0" : "=r"(kernel_stack));

    // Ensure we got a terminal
    if (terminal_request.response == NULL
     || terminal_request.response->terminal_count < 1) {
        done();
    }

    framebuffer_init();
    terminal_init();

    for (int x = 10; x < 100; x++) 
        drawhoriline(10, x, x + 10 * 3, torgb(255, 1, 1));

    // prints
    terminal_print("amogusussus\n");
    terminal_print("susususussamogus\n");
    //terminal_reset();
    terminal_print("BAKA BAKA BAKA");

    // hang...
    done();
}