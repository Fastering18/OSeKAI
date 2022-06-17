#ifndef H_FRAMEBUFFERDRIVER
#define H_FRAMEBUFFERDRIVER 1

#include <limine.h>

extern struct limine_framebuffer **framebuffers;
extern struct limine_framebuffer *default_frm;
extern uint64_t frm_count;

uint32_t torgb(uint32_t r, uint32_t g, uint64_t b);

void _putpixel(uint32_t x, uint32_t y, uint32_t colour, struct limine_framebuffer *frm);
void putpixel(uint32_t x, uint32_t y, uint32_t colour);
void drawvertline(uint32_t x, uint32_t y, uint32_t dy, uint32_t colour);
void drawhoriline(uint32_t x, uint32_t y, uint32_t dx, uint32_t colour);

void framebuffer_init();

#endif