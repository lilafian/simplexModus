#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../boot/limine.h"

#define PANIC_KERNEL_INIT_MIN 0x01
#define PANIC_KERNEL_INIT_MAX 0xEF
#define PANIC_KERNEL_MEMORY_MIN 0xF0
#define PANIC_KERNEL_MEMORY_MAX 0xEFF
#define PANIC_KERNEL_FS_MIN 0xF00
#define PANIC_KERNEL_FS_MAX 0x19FF
#define PANIC_KERNEL_DEVICES_MIN 0x1A00
#define PANIC_KERNEL_DEVICES_MAX 0x28FF
#define PANIC_DEVICE_SPECIFIC_MIN 0x2900
#define PANIC_DEVICE_SPECIFIC_MAX 0x29FF
#define PANIC_KERNEL_SECURITY_MIN 0x2A00
#define PANIC_KERNEL_SECURITY_MAX 0x2CFF
#define PANIC_KERNEL_MISC_MIN 0x2D00
#define PANIC_KERNEL_MISC_MAX 0x3CFF
#define PANIC_KERNEL_FUTURE_RESERVED_MIN 0x3D00 // never use this macro for panic codes
#define PANIC_KERNEL_FUTURE_RESERVED_MAX 0xEFFF // or this one
#define PANIC_PROGRAM_SPECIFIC_MIN 0xF000
#define PANIC_PROGRAM_SPECIFIC_MAX 0xFFFF
#define PANIC_CUSTOM_KERNEL_MIN 0x10000
#define PANIC_CUSTOM_KERNEL_MAX 0x10EFF

typedef struct {
    bool panicked;
    struct limine_framebuffer* framebuffer;
    void* display_font;
} PANIC_INFORMATION;

void kpanic(uint32_t error_code, char* error_description);
void kpanic_init(struct limine_framebuffer* framebuffer, void* display_font);
char* get_panic_type(uint32_t error_code);

extern PANIC_INFORMATION global_panic_information;
