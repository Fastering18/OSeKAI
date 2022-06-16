#include <stdint.h>
#include <stddef.h>

int div_roundup(int a , int b) {
	return (a + (b - 1)) / b;
}

uint64_t align_up(uint64_t value, uint64_t alignment) {
	return div_roundup(value, alignment) * alignment;
}

uint64_t align_down(uint64_t value, uint64_t alignment) {
	return (value / alignment) * alignment;
}

void *memset(void *s, int c,  unsigned int len)
{
    unsigned char* p=s;
    while(len--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}

void bitreset(void* bitmap, uint64_t index) {
    uint64_t ffbitmap = (uint64_t)(bitmap);
	uint64_t* fbitmap = &ffbitmap;
	size_t bits_type = sizeof(uint64_t) * 8;
	size_t test_index = index % bits_type;
	fbitmap[index / bits_type] &= ~((uint64_t)(1) << test_index);
}