/*
 * simplexModus/src/memory/paging/pageTableManager.c
 * copyright (c) 2025 lilaf, GNU General Public License v3.0.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * paging struct declarations
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool present : 1;
    bool read_write : 1;
    bool user_super : 1;
    bool write_through : 1;
    bool cache_disabled : 1;
    bool accessed : 1;
    bool ignore_0 : 1;
    bool larger_pages : 1;
    bool ignore_1: 1;
    uint8_t available : 3;
    uint64_t address : 52;
} PAGE_DIRECTORY_ENTRY;

typedef struct {
    PAGE_DIRECTORY_ENTRY entries[512];
}__attribute__((aligned(0x1000))) PAGE_TABLE;
