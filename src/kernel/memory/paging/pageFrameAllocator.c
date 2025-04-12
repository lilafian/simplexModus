/*
 * simplexModus/src/memory/paging/pageFrameAllocator.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * page frame allocator implementation
 */

#include "pageFrameAllocator.h"
#include "../../misc/panic/panic.h"
#include "../../common/klog.h"

uint64_t free_memory;
uint64_t reserved_memory;
uint64_t used_memory;
uint64_t total_memory;
extern uint64_t hhdm_offset = 0;

bool initialized = false;
PAGE_FRAME_ALLOCATOR* global_allocator;

void pfa_readEfiMemoryMap(PAGE_FRAME_ALLOCATOR* allocator, uint64_t memory_map_entry_count, struct limine_memmap_entry** memory_map_entries) {
    if (initialized) return;

    if (hhdm_offset == NULL) {
        klog("\033[31mcould not read memory map, hhdm_offset is null.\033[37m\n");
        return;
    }

    initialized = true;
    
    void* largest_free_segment = NULL;
    size_t largest_free_segment_size = 0;

    for (uint64_t i = 0; i < memory_map_entry_count; i++) {
        if (memory_map_entries[i]->type == LIMINE_MEMMAP_USABLE) {
            if (memory_map_entries[i]->length > largest_free_segment_size) {
                largest_free_segment = (void*)memory_map_entries[i]->base;
                largest_free_segment_size = memory_map_entries[i]->length;
            }
        }
    }

    if (largest_free_segment == NULL) {
        kpanic(PANIC_KERNEL_MEMORY_MIN + 0x0, "NO USABLE MEMORY IN MEMORY MAP");
    }

    uint64_t memory_size = getMemorySize(memory_map_entry_count, memory_map_entries);
    total_memory = memory_size;
    free_memory = memory_size;
    used_memory = 0;
    reserved_memory = 0;

    uint64_t bitmap_size = memory_size / 4096 / 8 + 1;

    pfa_initBitmap(allocator, bitmap_size, largest_free_segment);
    void* bitmap_physical_address = (void*)((uint64_t)&allocator->page_bitmap.buffer - hhdm_offset);
    pfa_lockPages(allocator, bitmap_physical_address, allocator->page_bitmap.size / 4096 + 1);

    for (uint64_t i = 0; i < memory_map_entry_count; i++) {
        // 
        if ((memory_map_entries[i]->type != LIMINE_MEMMAP_USABLE && memory_map_entries[i]->type != LIMINE_MEMMAP_RESERVED) || memory_map_entries[i]->type == LIMINE_MEMMAP_KERNEL_AND_MODULES) { // reserved memory should be ignored as it is millions of pages (no idea why, it takes up more than available space), also just ensure that the kernel is reserved
            pfa_reservePages(allocator, (void*)memory_map_entries[i]->base, memory_map_entries[i]->length / 4096);
        }

        
    }
    // reserve first page since limine doesnt like it
    pfa_reservePages(allocator, (void*)0, 1);

    if (free_memory == 0) {
        kpanic(PANIC_KERNEL_MEMORY_MIN + 0x1, "OUT OF MEMORY");
    }
}

void pfa_initBitmap(PAGE_FRAME_ALLOCATOR* allocator, size_t bitmap_size, void* buffer_address) {
    allocator->page_bitmap.size = bitmap_size;
    allocator->page_bitmap.buffer = (uint8_t*)((uint64_t)buffer_address + hhdm_offset);

    for (uint64_t i = 0; i < bitmap_size; i++) {
        *(uint8_t*)(allocator->page_bitmap.buffer + i) = 0;
    }
}


void pfa_freePage(PAGE_FRAME_ALLOCATOR* allocator, void* address) {
    uint64_t index = ((uint64_t)address) / 4096;
    if (bitmap_get(&allocator->page_bitmap, index) == false) return;
    bitmap_set(&allocator->page_bitmap, index, false);
    if (used_memory < 4096 || free_memory + 4096 > total_memory) return;  
    free_memory += 4096;
    used_memory -= 4096;
}

void pfa_freePages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        pfa_freePage(allocator, (void*)((uint64_t)address + (i * 4096))); // cast to uint64 so that we can do math on it and then cast back to void* for the correct type
    }
}

void pfa_lockPage(PAGE_FRAME_ALLOCATOR* allocator, void* address) {
    uint64_t index = ((uint64_t)address) / 4096;
    if (bitmap_get(&allocator->page_bitmap, index) == true) return;
    bitmap_set(&allocator->page_bitmap, index, true);
    if (free_memory < 4096 || used_memory + 4096 > total_memory) return;
    free_memory -= 4096;
    used_memory += 4096;
}

void pfa_lockPages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        pfa_lockPage(allocator, (void*)((uint64_t)address + (i * 4096))); // cast to uint64 so that we can do math on it and then cast back to void* for the correct type
    }
}

void pfa_releasePage(PAGE_FRAME_ALLOCATOR* allocator, void* address) {
    uint64_t index = ((uint64_t)address) / 4096;
    if (bitmap_get(&allocator->page_bitmap, index) == false) return;
    bitmap_set(&allocator->page_bitmap, index, false);
    if (reserved_memory < 4096 || free_memory + 4096 > total_memory) return;
    free_memory += 4096;
    reserved_memory -= 4096;
}

void pfa_releasePages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        pfa_releasePage(allocator, (void*)((uint64_t)address + (i * 4096))); // cast to uint64 so that we can do math on it and then cast back to void* for the correct type
    }
}

void pfa_reservePage(PAGE_FRAME_ALLOCATOR* allocator, void* address) {
    uint64_t index = (uint64_t)address / 4096;
    if (bitmap_get(&allocator->page_bitmap, index) == true) return;
    bitmap_set(&allocator->page_bitmap, index, true);
    if (free_memory < 4096 || reserved_memory + 4096 > total_memory) return;
    free_memory -= 4096;
    reserved_memory += 4096;
}

void pfa_reservePages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        pfa_reservePage(allocator, (void*)((uint64_t)address + (i * 4096))); // cast to uint64 so that we can do math on it and then cast back to void* for the correct type
    }
}

void* pfa_requestPage(PAGE_FRAME_ALLOCATOR* allocator) {
    // optimize this
    for (uint64_t i = 0; i < allocator->page_bitmap.size * 8; i++) {
        if (bitmap_get(&allocator->page_bitmap, i) == true) continue;
        pfa_lockPage(allocator, (void*)(i * 4096));
        return (void*)(i * 4096 + hhdm_offset);
    }

    // add swapfile implementation here
    return NULL;
}

uint64_t pfa_getFreeMemory() {
    return free_memory;
}

uint64_t pfa_getUsedMemory() {
    return used_memory;
}

uint64_t pfa_getReservedMemory() {
    return reserved_memory;
}
