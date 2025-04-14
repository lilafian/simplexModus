/*
 * simplexModus/src/memory/memory.h
 * copyright (c) 2025 lilaf, GNU General Public License v3.0.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * common memory function declarations
 */

#pragma once

#include <stdint.h>
#include "../boot/limine.h"

uint64_t getMemorySize(uint64_t memory_map_entry_count, struct limine_memmap_entry** memory_map_entries);
