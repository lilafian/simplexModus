/*
 * simplexModus/src/kernel/graphics/framebuffer.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * framebuffer renderer implementation
 */

#include "framebuffer.h"

void drawPixel(uint64_t posX, uint64_t posY, struct limine_framebuffer *framebuffer, uint32_t color) {
    volatile uint32_t *fb_ptr = (volatile uint32_t *)(uintptr_t)framebuffer->address;
    fb_ptr[posY * (framebuffer->pitch / 4) + posX] = color;
}

void drawRect(uint64_t posX, uint64_t posY, uint64_t width, uint64_t height, struct limine_framebuffer *framebuffer, uint32_t color) {
    int incY = 0;
    for (uint64_t i = 0; i < height; i++) {
        int incX = 0;
        for (uint64_t i = 0; i < width; i++) {
            drawPixel(posX + incX, posY + incY, framebuffer, color);
            incX++;
        }
        incY++;
    }
}

void clearFramebuffer(struct limine_framebuffer* framebuffer) {
    volatile uint32_t *fb_ptr = (volatile uint32_t *)(uintptr_t)framebuffer->address;
    size_t pixel_count = framebuffer->width * framebuffer->height;
    
    for (size_t i = 0; i < pixel_count; i++) {
        fb_ptr[i] = 0;
    }
}

