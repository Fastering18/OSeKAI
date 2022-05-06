#include <limine.h>

#if LVL5_PAGING
extern volatile struct limine_5_level_paging_request _5_level_paging_request;
#endif

extern volatile struct limine_terminal_request terminal_request;
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile struct limine_smp_request smp_request;

