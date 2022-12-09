#include <limine.h>
#include <kernel.h>
#include <stdint.h>
#include <stddef.h>
#include "mem.h"

#define MEM_PAGE_SIZE 0x1000

uint64_t FREE_PAGES;
void * pmm_bitmap;
struct limine_memmap_response *memmap;

void mem_init() {
    memmap = memmap_request.response;
    uint64_t highestaddr;

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE &&
        memmap->entries[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }
        uint64_t top = memmap->entries[i]->base + memmap->entries[i]->length;
        if (top > highestaddr) highestaddr = top;
    }
    uint64_t pmm_avl_page_count = highestaddr / MEM_PAGE_SIZE;
    uint64_t bitmap_size = align_up(pmm_avl_page_count / 8, MEM_PAGE_SIZE);
    terminal_print("|- Bitmap size: ");
    terminal_printi(bitmap_size);
    terminal_print("\n");

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }
        if (memmap->entries[i]->length >= bitmap_size) {
            pmm_bitmap = (void *)(memmap->entries[i]->base + 0xffff800000000000);
            memset(pmm_bitmap, 0xff, bitmap_size);

            memmap->entries[i]->length -= bitmap_size;
            memmap->entries[i]->base += bitmap_size;

            break;
        }
    }


    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }
        for (uint64_t j = 0; j < memmap->entries[i]->length; j += MEM_PAGE_SIZE) {
            FREE_PAGES++;
            bitreset(pmm_bitmap, (memmap->entries[i]->base + j) / MEM_PAGE_SIZE);
        }
    }

    terminal_print("|- Free pages: ");
    terminal_printi(FREE_PAGES);
    terminal_print("\n");
}