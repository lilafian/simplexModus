/*
 * simplexModus/src/memory/paging/pageTableManager.h
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * page table manager struct and function declarations
 */

#pragma once

#include "paging.h"

typedef struct {
    PAGE_TABLE* page_map_lv4;
} PAGE_TABLE_MANAGER;

void ptm_init(PAGE_TABLE_MANAGER* page_table_manager, PAGE_TABLE* page_map_lv4_address);
void ptm_mapMemory(PAGE_TABLE_MANAGER* page_table_manager, void* virtual_memory, void* physical_memory, uint64_t hhdm_offset);
