#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../boot/limine.h"

void drawPixel(uint64_t posX, uint64_t posY, struct limine_framebuffer *framebuffer, uint32_t color);
