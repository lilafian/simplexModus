/*
 * simplexModus/src/kernel/graphics/psfFonts.h
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * framebuffer pc screen font renderer declarations
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "framebuffer.h"
#include "../boot/limine.h"

#define PSF2_MAGIC 0x864ab572

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
} PSF2_header;

void drawChar(int x, int y, char c, uint32_t color, const void* font_data, struct limine_framebuffer *framebuffer);
void drawString(int x, int y, const char* string, uint32_t color, const void* font_data, struct limine_framebuffer *framebuffer, bool no_color);
