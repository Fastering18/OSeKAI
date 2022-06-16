#include <limine.h>
#include <kernel.h>

struct limine_framebuffer **framebuffers;
struct limine_framebuffer *default_frm;
uint64_t frm_count;

uint32_t cursorbuffer[16 * 19];
uint32_t cursorbuffersecond[16 * 19];

//bool mousedrawn;

uint32_t torgb(uint32_t r, uint32_t g, uint64_t b) {
    return (uint32_t)((r << 16) | (g << 8) | b);
}

void _putpixel(uint32_t x, uint32_t y, uint32_t colour, struct limine_framebuffer *frm) {
    *(uint32_t *)( (uint64_t)(frm->address) + (x * 4) + (y * (frm->pitch / (frm->bpp / 8)) * 4) ) = colour;
}
void putpixel(uint32_t x, uint32_t y, uint32_t colour) {
    *(uint32_t *)((uint64_t)(default_frm->address) + (x * 4) + (y * (default_frm->pitch / (default_frm->bpp / 8)) * 4)) = colour;
}

void drawvertline(uint32_t x, uint32_t y, uint32_t dy, uint32_t colour) {
    for (uint32_t i = 0; i < dy; i++) putpixel(x, y + i, colour);
}

void drawhoriline(uint32_t x, uint32_t y, uint32_t dx, uint32_t colour) {
    for (uint32_t i = 0; i < dx; i++) putpixel(x + i, y, colour);
}

void framebuffer_init()
{
    framebuffers = framebuffer_request.response->framebuffers;
    default_frm = framebuffers[0];
    frm_count = framebuffer_request.response->framebuffer_count;
}