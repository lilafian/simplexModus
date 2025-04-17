#pragma once

#include <stdint.h>

typedef struct {
    uint8_t boot_jmp[3];
    unsigned char oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t root_directory_entries;
    uint16_t total_sectors; // if this value is 0, check large_sector_count, total_sectors == 0 means that there are more than 65,535 sectors.
    uint8_t media_descriptor_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track; // don't trust this
    uint16_t heads; // don't trust this
    uint32_t hidden_sectors;
    uint32_t large_sector_count;
    FAT32_EXTENDED_BOOT_RECORD extended_boot_record;
}__attribute__((packed)) FAT32_BOOT_SECTOR;

typedef struct {
    uint32_t sectors_per_fat;
    uint16_t flags;
    uint8_t fat_version[2]; // high byte is the major version, low byte is the minor version
    uint32_t root_cluster_number;
    uint16_t fsinfo_sector_number;
    uint16_t backup_boot_sector_number;
    char reserved1[12];
    uint8_t drive_number; // 0x00-0x7F for floppy disk, 0x80-0xFF for hard disk
    char reserved2;
    uint8_t signature; // must be 0x28 or 0x29
    uint32_t volumeid; // can be ignored
    char volume_label[11];
    char system_identifier[8]; // always "FAT32   "
    char boot_code[420];
    uint16_t boot_signature; // 0xAA55
}__attribute__((packed)) FAT32_EXTENDED_BOOT_RECORD;

typedef struct {
    uint32_t signature1; // must be 0x41615252
    char reserved1[480];
    uint32_t signature2; // must be 0x61417272
    uint32_t last_known_free_cluster_count; // if 0xFFFFFFFF, free count is unknown. could be incorrect, range check it ( <= volume cluster count )
    uint32_t available_cluster_start; // if 0xFFFFFFFF, start at 2. range check this
    char reserved2[12];
    uint32_t signature3; // must be 0xAA550000
}__attribute__((packed)) FAT32_FSINFO;
