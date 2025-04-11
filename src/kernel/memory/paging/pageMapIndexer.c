/*
 * simplexModus/src/memory/paging/pageMapIndexer.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * page map indexer implementation
 */

#include "pageMapIndexer.h"

void pmi_init(PAGE_MAP_INDEXER* page_map_indexer, uint64_t virtual_address) {
    virtual_address >>= 12;
    page_map_indexer->p_index = virtual_address & 0x1ff;
    virtual_address >>= 9;
    page_map_indexer->pt_index = virtual_address & 0x1ff;
    virtual_address >>= 9;
    page_map_indexer->pd_index = virtual_address & 0x1ff;
    virtual_address >>= 9;
    page_map_indexer->pdp_index = virtual_address & 0x1ff;
}
