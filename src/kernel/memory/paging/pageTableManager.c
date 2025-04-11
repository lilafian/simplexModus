/*
 * simplexModus/src/memory/paging/pageTableManager.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * page table manager implementation
 */


#include "pageTableManager.h"
#include "pageMapIndexer.h"
#include "pageFrameAllocator.h"
#include "../../common/cfuncs.h"
#include "../../panic/panic.h"
#include <stdint.h>

void ptm_init(PAGE_TABLE_MANAGER* page_table_manager, PAGE_TABLE* page_map_lv4_address) {
    page_table_manager->page_map_lv4 = page_map_lv4_address; 
}

void ptm_mapMemory(PAGE_TABLE_MANAGER* page_table_manager, void* virtual_memory, void* physical_memory, uint64_t hhdm_offset) {
    if (page_table_manager->page_map_lv4 == NULL) {
        kpanic(PANIC_KERNEL_MEMORY_MIN + 0x2, "PAGE MAP LEVEL 4 IS NULL");
    }

    PAGE_MAP_INDEXER indexer;
    PAGE_MAP_INDEXER* indexer_ptr = &indexer;
    pmi_init(indexer_ptr, (uint64_t)virtual_memory);

    PAGE_DIRECTORY_ENTRY page_directory_entry;

    page_directory_entry = page_table_manager->page_map_lv4->entries[indexer.pdp_index];
    PAGE_TABLE* page_directory_pointer;
    if (!page_directory_entry.present) {
        page_directory_pointer = (PAGE_TABLE*)((uint64_t)pfa_requestPage(global_allocator) + hhdm_offset);
        memset(page_directory_pointer, 0, 4096);
        page_directory_entry.address = ((uint64_t)page_directory_pointer - hhdm_offset) >> 12;
        page_directory_entry.present = true;
        page_directory_entry.read_write = true;
        page_table_manager->page_map_lv4->entries[indexer.pdp_index] = page_directory_entry;
    } else {
        page_directory_pointer = (PAGE_TABLE*)(((uint64_t)page_directory_entry.address << 12) + hhdm_offset);
    }

    page_directory_entry = page_directory_pointer->entries[indexer.pd_index];
    PAGE_TABLE* page_directory;
    if (!page_directory_entry.present) {
        page_directory = (PAGE_TABLE*)((uint64_t)pfa_requestPage(global_allocator) + hhdm_offset);
        memset(page_directory, 0, 4096);
        page_directory_entry.address = ((uint64_t)page_directory - hhdm_offset) >> 12;
        page_directory_entry.present = true;
        page_directory_entry.read_write = true;
        page_directory_pointer->entries[indexer.pd_index] = page_directory_entry;
    } else {
        page_directory = (PAGE_TABLE*)(((uint64_t)page_directory_entry.address << 12) + hhdm_offset);
    }

    page_directory_entry = page_directory->entries[indexer.pt_index];
    PAGE_TABLE* page_table;
    if (!page_directory_entry.present) {
        page_table = (PAGE_TABLE*)((uint64_t)pfa_requestPage(global_allocator) + hhdm_offset);
        memset(page_table, 0, 4096);
        page_directory_entry.address = ((uint64_t)page_table - hhdm_offset) >> 12;
        page_directory_entry.present = true;
        page_directory_entry.read_write = true;
        page_directory->entries[indexer.pt_index] = page_directory_entry;
    } else {
        page_table = (PAGE_TABLE*)(((uint64_t)page_directory_entry.address << 12) + hhdm_offset);
    }

    page_directory_entry = page_table->entries[indexer.p_index];
    page_directory_entry.address = (uint64_t)physical_memory >> 12;
    page_directory_entry.present = true;
    page_directory_entry.read_write = true;
    page_table->entries[indexer.p_index] = page_directory_entry;
}
