#pragma once

#include <stdint.h>
#include "../boot/limine.h"

uint64_t getMemorySize(uint64_t memory_map_entry_count, struct limine_memmap_entry** memory_map_entries);
