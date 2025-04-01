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
