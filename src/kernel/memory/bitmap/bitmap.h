/*
 * simplexModus/src/memory/bitmap/bitmap.h
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * bitmap function and struct declarations
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    size_t size;
    uint8_t* buffer;
} BITMAP;

bool bitmap_get(BITMAP* bitmap, uint64_t index);
void bitmap_set(BITMAP* bitmap, uint64_t index, bool value);
