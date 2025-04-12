/*
 * simplexModus/src/kernel/misc/panic/panic.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * kernel panic function implementations
 */

#include "panic.h"
#include "../../console/basicConsole/basicConsole.h"
#include "../../common/cfuncs.h"

PANIC_INFORMATION global_panic_information;

void kpanic(uint32_t error_code, char* error_description) {
    asm volatile("cli");

    if (global_panic_information.framebuffer == NULL || global_panic_information.display_font == NULL || global_panic_information.panicked == true) {
        while(true) {
            asm volatile("hlt");
        }
    }

    BASIC_CONSOLE panic_console;
    bcon_init(&panic_console, global_panic_information.framebuffer, global_panic_information.display_font, "kernel panic display console");

    bcon_append(&panic_console, "\033[31m(x_x) kernel panic\033[37m\n\na critical error has occurred and the operating system has halted.\n\nerror code: 0x", true);
    bcon_append(&panic_console, itoa(error_code, 16), true);
    bcon_append(&panic_console, "\nerror description: ", true);
    bcon_append(&panic_console, error_description, true);
    bcon_append(&panic_console, "\nerror type: ", true);
    bcon_append(&panic_console, get_panic_type(error_code), true);
    bcon_append(&panic_console, "\n\nyou can safely shut down using the power button.", true);

    global_panic_information.panicked = true;

    while(true) {
        asm volatile("hlt");
    }
}

void kpanic_init(struct limine_framebuffer* framebuffer, void* display_font) {
    global_panic_information.panicked = false;
    global_panic_information.framebuffer = framebuffer;
    global_panic_information.display_font = display_font;
}

char* get_panic_type(uint32_t error_code) {
    if (error_code < 0x01 || error_code > PANIC_CUSTOM_KERNEL_MAX) {
        return "PANIC_INVALID";
    } else if (error_code >= PANIC_KERNEL_INIT_MIN && error_code <= PANIC_KERNEL_INIT_MAX) {
        return "PANIC_KERNEL_INIT";
    } else if (error_code >= PANIC_KERNEL_MEMORY_MIN && error_code <= PANIC_KERNEL_MEMORY_MAX) {
        return "PANIC_KERNEL_MEMORY";
    } else if (error_code >= PANIC_KERNEL_FS_MIN && error_code <= PANIC_KERNEL_FS_MAX) {
        return "PANIC_KERNEL_FS";
    } else if (error_code >= PANIC_KERNEL_DEVICES_MIN && error_code <= PANIC_KERNEL_DEVICES_MAX) {
        return "PANIC_KERNEL_DEVICES";
    } else if (error_code >= PANIC_DEVICE_SPECIFIC_MIN && error_code <= PANIC_DEVICE_SPECIFIC_MAX) {
        return "PANIC_DEVICE_SPECIFIC";
    } else if (error_code >= PANIC_KERNEL_SECURITY_MIN && error_code <= PANIC_KERNEL_SECURITY_MAX) {
        return "PANIC_KERNEL_SECURITY";
    } else if (error_code >= PANIC_KERNEL_MISC_MIN && error_code <= PANIC_KERNEL_MISC_MAX) {
        return "PANIC_KERNEL_MISC";
    } else if (error_code >= PANIC_KERNEL_FUTURE_RESERVED_MIN && error_code <= PANIC_KERNEL_FUTURE_RESERVED_MAX) {
        return "PANIC_KERNEL_FUTURE_RESERVED";
    } else if (error_code >= PANIC_PROGRAM_SPECIFIC_MIN && error_code <= PANIC_PROGRAM_SPECIFIC_MAX) {
        return "PANIC_PROGRAM_SPECIFIC";
    } else if (error_code >= PANIC_CUSTOM_KERNEL_MIN && error_code <= PANIC_CUSTOM_KERNEL_MAX) {
        return "PANIC_CUSTOM_KERNEL";
    }
    return "PANIC_INVALID";
}
