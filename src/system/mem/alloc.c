#include <limine.h>
#include <kernel.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "mem.h"

#define MEM_PAGE_SIZE 0x1000

uint64_t FREE_PAGES;
void * pmm_bitmap;
struct limine_memmap_response *memmap;

static uintptr_t highestaddr = 0;
static uintptr_t mem_usable_top = 0;
static size_t lastI = 0;

static size_t usedmem = 0;
static size_t usablemem = 0;
static size_t totalmem = 0;

size_t freemem() 
{
    return usablemem - usedmem;
}

void mem_info()
{
    size_t free = freemem();
    printf("|- Free pages: %d | Total ram: %dMB | Free ram: %dMB\n", FREE_PAGES, totalmem/1024/1024, free/1024/1024);
}

void mem_init() {
    memmap = memmap_request.response;

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        /*if (memmap->entries[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE &&
        memmap->entries[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }*/
        uint64_t top = memmap->entries[i]->base + memmap->entries[i]->length;
        //freeRam += memmap->entries[i]->length;

        if (top > highestaddr) highestaddr = top;
        switch (memmap->entries[i]->type)
        {
            case LIMINE_MEMMAP_USABLE:
                usablemem += memmap->entries[i]->length;
                if (top > mem_usable_top) mem_usable_top = top;
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            case LIMINE_MEMMAP_KERNEL_AND_MODULES:
                totalmem += memmap->entries[i]->length;
                break;
        }
    }
    uint64_t pmm_avl_page_count = mem_usable_top / MEM_PAGE_SIZE;
    uint64_t bitmap_size = align_up(pmm_avl_page_count / 8, MEM_PAGE_SIZE);
    printf("|- Bitmap size: %d\n", bitmap_size);

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }
        if (memmap->entries[i]->length >= bitmap_size) {
            pmm_bitmap = (void *)(memmap->entries[i]->base + 0xffff800000000000);
            memset(pmm_bitmap, 0xff, bitmap_size);

            memmap->entries[i]->length -= bitmap_size;
            memmap->entries[i]->base += bitmap_size;
            usedmem += bitmap_size;
            break;
        }
    }


    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }
        for (uint64_t j = 0; j < memmap->entries[i]->length; j += MEM_PAGE_SIZE) {
            FREE_PAGES++;
            bitset(pmm_bitmap, (memmap->entries[i]->base + j) / MEM_PAGE_SIZE, false);
        }
    }

    highestaddr = align_up(highestaddr, 0x40000000);
    mem_info();
}

static void *inner_alloc(size_t count, size_t limit)
{
    size_t p = 0;

    while (lastI < limit)
    {
        if (bitget(pmm_bitmap, lastI++) == false)
        {
            if (++p == count)
            {
                size_t page = lastI - count;
                for (size_t i = page; i < lastI; i++) bitset(pmm_bitmap, i, true);
                return (void*)(page * MEM_PAGE_SIZE);
            }
        }
        else p = 0;
    }
    return NULL;
}

void *alloc(size_t count)
{
    size_t i = lastI;
    void *ret = inner_alloc(count, mem_usable_top / MEM_PAGE_SIZE);
    if (ret == NULL)
    {
        lastI = 0;
        ret = inner_alloc(count, i);
        if (ret == NULL) {
            printf("Out of memory!\n");
           // __asm__ ("int $0xE");
        }
        return NULL;
    }
    memset((void*)((uint64_t)(ret) + hhdm_offset), 0, count * MEM_PAGE_SIZE);

    usedmem += count * MEM_PAGE_SIZE;
    return ret;
}

void free(void *ptr, size_t count)
{
    if (ptr == NULL) return;

    size_t page = (size_t)(ptr) / MEM_PAGE_SIZE;
    for (size_t i = page; i < page + count; i++) bitset(pmm_bitmap, i, false);
    if (lastI > page) lastI = page;

    usedmem -= count * MEM_PAGE_SIZE;
}

void *realloc(void *ptr, size_t oldcount, size_t newcount)
{
    if (ptr == NULL) return alloc(newcount);

    if (!newcount)
    {
        free(ptr, oldcount);
        return NULL;
    }

    if (newcount < oldcount) oldcount = newcount;

    void *newptr = alloc(newcount);
    memcpy(newptr, ptr, oldcount);
    free(ptr, 1);
    return newptr;
}