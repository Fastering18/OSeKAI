#ifndef H_ALLOC
#define H_ALLOC 1

#include <limine.h>

extern uint64_t FREE_PAGES;
extern void * pmm_bitmap;

extern struct limine_memmap_response *memmap;

void mem_init();
void mem_info();

void *alloc(size_t count);
void free(void *ptr, size_t count);
void *realloc(void *ptr, size_t oldcount, size_t newcount);

#endif