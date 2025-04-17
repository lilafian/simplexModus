/*
 * simplexModus/src/kernel/main.c
 * copyright (c) 2025 lilaf, GNU General Public License v3.0.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * simplexModus kernel initialization and entry point
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "boot/limine.h"
#include "common/cfuncs.h"
#include "graphics/psfFonts.h"
#include "console/basicConsole/basicConsole.h"
#include "memory/memory.h"
#include "memory/bitmap/bitmap.h"
#include "memory/paging/pageFrameAllocator.h"
#include "memory/paging/paging.h"
#include "misc/panic/panic.h"
#include "common/klog.h"

// limine requests
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

static void halt(void) {
    while (true) {
        asm("hlt");
    }
}

extern uint8_t _binary_assets_fonts_zap_ext_vga16_psf_start[];
extern uint8_t _binary_assets_fonts_zap_ext_vga16_psf_end[];
extern uint8_t _binary_assets_fonts_zap_ext_vga16_psf_size;

// ! KERNEL ENTRY POINT !
void kernel_main(void) {
    // ensure revision supported
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        halt();
    }

    // ensure framebuffer
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
        halt();
    }

    // fetch framebuffer
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    void* font_data = &_binary_assets_fonts_zap_ext_vga16_psf_start;
    // set up console and panic before everything else so that we can panic for errors
    BASIC_CONSOLE kconsole;
    BASIC_CONSOLE* kconsole_ptr = &kconsole;
    bcon_init(kconsole_ptr, framebuffer, font_data, "simplexModus early kernel console");
    kpanic_init(framebuffer, font_data);
    klog_init(KERNEL_LOG_MODE_CON_DISPLAYED, kconsole_ptr, "/system/log/klog-latest.log"); 

    // ensure memory map
    if (memmap_request.response == NULL || memmap_request.response->entries == NULL) {
        kpanic(PANIC_KERNEL_INIT_MIN + 0x0, "MEMORY MAP LOAD FAILED");
    }

    // ensure hhdm offset
    if (hhdm_request.response == NULL) {
        kpanic(PANIC_KERNEL_INIT_MIN + 0x1, "HHDM OFFSET LOAD FAILED");
    }

    const struct limine_memmap_response* memmap_response = memmap_request.response;
    const struct limine_hhdm_response* hhdm_response = hhdm_request.response;

    klog("\033[32msimplexModus \033[36mv0\n");

    klogf("\033[37mframebuffer dimensions: %dx%d\n", framebuffer->width, framebuffer->height);

    klog("creating global page frame allocator\n");
    PAGE_FRAME_ALLOCATOR allocator;
    PAGE_FRAME_ALLOCATOR* allocator_ptr = &allocator;
    global_allocator = allocator_ptr;
    hhdm_offset = hhdm_response->offset;
    pfa_readEfiMemoryMap(allocator_ptr, memmap_response->entry_count, memmap_response->entries);
    klog("created global page frame allocator!\n");

    const struct limine_rsdp_response* rsdp_response = rsdp_request.response;

    // DO NOT CALL ANY FUNCTIONS THAT USE THE GLOBAL ALLOCATOR ABOVE THIS LINE! THE KERNEL WILL CRASH!
    // below here is safe :)
    
    klogf("total memory size: \033[36m%d KiB\033[37m\n", getMemorySize(memmap_response->entry_count, memmap_response->entries) / 1024);
    klogf("free memory: \033[36m%d KiB\033[37m\n", pfa_getFreeMemory() / 1024);
    klogf("used memory: \033[36m%d KiB\033[37m\n", pfa_getUsedMemory() / 1024);
    klogf("reserved memory: \033[36m%d KiB\033[37m\n", pfa_getReservedMemory() / 1024);

    klogf("rsdp address: 0x%p\n", rsdp_response->address);
    klog("todo: fix page map, make sure to map the hhdm kernel mapping to the same address, but i can still map everything else pa=va");

    // done, hang
    halt();
}
