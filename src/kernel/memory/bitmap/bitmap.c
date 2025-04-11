/*
 * simplexModus/src/memory/bitmap/bitmap.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * bitmap function implementations
 */

#include "bitmap.h"

bool bitmap_get(BITMAP* bitmap, uint64_t index) {
    uint64_t byte_index = index / 8; // byte that the index is in
    uint8_t bit_index = index % 8; // the bit of byte_index that we want to get
    uint8_t bit_indexer = 0b10000000 >> bit_index; // use bit_indexer to select the bit we need (e.g. bit_index = 4, bit_indexer = 0b00001000)

    if ((bitmap->buffer[byte_index] & bit_indexer) > 0) {
        return true;
    }
    return false;
}

void bitmap_set(BITMAP* bitmap, uint64_t index, bool value) {
    uint64_t byte_index = index / 8; // byte that the index is in
    uint8_t bit_index = index % 8; // the bit of byte_index that we want to get
    uint8_t bit_indexer = 0b10000000 >> bit_index; // use bit_indexer to select the bit we need (e.g. bit_index = 4, bit_indexer = 0b00001000)

    bitmap->buffer[byte_index] &= ~bit_indexer;
    if (value) bitmap->buffer[byte_index] |= bit_indexer;
}
