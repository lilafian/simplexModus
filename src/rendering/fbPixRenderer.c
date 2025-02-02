#include "fbPixRenderer.h"

void drawPixel(uint64_t posX, uint64_t posY, struct limine_framebuffer *framebuffer, uint32_t color) {
    volatile uint32_t *fb_ptr = (volatile uint32_t *)(uintptr_t)framebuffer->address;
    fb_ptr[posY * (framebuffer->pitch / 4) + posX] = color;
}
