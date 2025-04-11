/*
 * simplexModus/src/kernel/graphics/framebuffer.h
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * framebuffer renderer declarations
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include "../boot/limine.h"

void drawPixel(uint64_t posX, uint64_t posY, struct limine_framebuffer *framebuffer, uint32_t color);
void drawRect(uint64_t posX, uint64_t posY, uint64_t width, uint64_t height, struct limine_framebuffer *framebuffer, uint32_t color);
void clearFramebuffer(struct limine_framebuffer *framebuffer);
