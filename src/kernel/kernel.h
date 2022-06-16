#include <limine.h>

static const uint64_t STACK_SIZE = 0x4000;

extern uint8_t *kernel_stack;
extern const char *cmdline;
extern uint64_t hhdm_offset;

#if LVL5_PAGING
extern volatile struct limine_5_level_paging_request _5_level_paging_request;
#endif

extern volatile struct limine_terminal_request terminal_request;
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile struct limine_memmap_request memmap_request;
extern volatile struct limine_smp_request smp_request;

