#pragma once

#include "../memory.h"
#include "../bitmap/bitmap.h"
#include "../../boot/limine.h"
#include <stdint.h>

typedef struct {
    BITMAP page_bitmap;
} PAGE_FRAME_ALLOCATOR;

void pfa_readEfiMemoryMap(PAGE_FRAME_ALLOCATOR* allocator, uint64_t memory_map_entry_count, struct limine_memmap_entry** memory_map_entries, uint64_t hhdm_offset);
void pfa_initBitmap(PAGE_FRAME_ALLOCATOR* allocator, size_t bitmap_size, void* buffer_address, uint64_t hhdm_offset);

void pfa_freePage(PAGE_FRAME_ALLOCATOR* allocator, void* address);
void pfa_freePages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t count);

void pfa_lockPage(PAGE_FRAME_ALLOCATOR* allocator, void* address);
void pfa_lockPages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t count);

void pfa_reservePage(PAGE_FRAME_ALLOCATOR* allocator, void* address);
void pfa_reservePages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t count);

void pfa_releasePage(PAGE_FRAME_ALLOCATOR* allocator, void* address);
void pfa_releasePages(PAGE_FRAME_ALLOCATOR* allocator, void* address, uint64_t hhdm_offset);

void* pfa_requestPage(PAGE_FRAME_ALLOCATOR* allocator);

uint64_t pfa_getFreeMemory();
uint64_t pfa_getUsedMemory();
uint64_t pfa_getReservedMemory();

extern PAGE_FRAME_ALLOCATOR* global_allocator;
