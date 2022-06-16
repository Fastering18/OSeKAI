#ifndef H_ALLOC
#define H_ALLOC

#include <limine.h>

extern uint64_t FREE_PAGES;
extern void * pmm_bitmap;

extern struct limine_memmap_response *memmap;

void mem_init();

#endif