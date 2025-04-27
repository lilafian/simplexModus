/*
 * simplexModus/src/kernel/memory/memory.c
 * copyright (c) 2025 lilaf, GNU General Public License v3.0.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * common memory function implementations
 */

#include "memory.h"

uint64_t getMemorySize(uint64_t memory_map_entry_count, struct limine_memmap_entry** memory_map_entries) {
    volatile int size = 0;

    for (uint64_t i = 0; i < memory_map_entry_count; i++) {
        if (memory_map_entries[i]->type == 0) {
            size += memory_map_entries[i]->length;
        }
    }

    return size;
}
