#ifndef H_MEMLIB
#define H_MEMLIB 1

#include <stdint.h>
#include <stddef.h>

int div_roundup(int a , int b);
uint64_t align_up(uint64_t value, uint64_t alignment);
uint64_t align_down(uint64_t value, uint64_t alignment);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c,  unsigned int len);

void bitreset(void* bitmap, uint64_t index);
void bitget(void* bitmap, uint64_t index);

#endif
