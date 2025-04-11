/*
 * simplexModus/src/memory/paging/pageMapIndexer.h
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * page map indexer struct and function declarations
 */


#pragma once

#include <stdint.h>

typedef struct {
    uint64_t pdp_index; // page directory pointer 
    uint64_t pd_index; // page directory
    uint64_t pt_index; // page table
    uint64_t p_index; // page
} PAGE_MAP_INDEXER;

void pmi_init(PAGE_MAP_INDEXER* page_map_indexer, uint64_t virtual_address);
