// simplexModus

// includes
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "boot/limine.h"
#include "cfuncs/cfuncs.h"
#include "rendering/psfFonts.h"
#include "console/basicConsole.h"
#include "memory/memory.h"
#include "memory/bitmap/bitmap.h"
#include "memory/paging/pageFrameAllocator.h"
#include "memory/paging/pageMapIndexer.h"
#include "memory/paging/paging.h"
#include "memory/paging/pageTableManager.h"

// limine requests
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0
};

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

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

// halt and catch fire
static void hcf(void) {
    for (;;) {
        asm("hlt");
    }
}

extern char _binary_assets_fonts_zap_ext_light32_psf_start;
extern char _binary_assets_fonts_zap_ext_light32_psf_end;
extern char _binary_assets_fonts_zap_ext_light32_psf_size;

// ! KERNEL ENTRY POINT !
void kernMain(void) {
    // ensure revision supported
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // ensure framebuffer
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // fetch framebuffer
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    void* font_data = &_binary_assets_fonts_zap_ext_light32_psf_start;
    const char* fb_width_str = itoa(framebuffer->width, 10);
    const char* fb_height_str = itoa(framebuffer->height, 10);

    // set up console before everything else so that we can output error logs (we know we can because the framebuffer works at this point)
    BASIC_CONSOLE kconsole;
    BASIC_CONSOLE* kconsole_ptr = &kconsole;
    bcon_init(kconsole_ptr, framebuffer, font_data);

    // ensure memory map
    if (memmap_request.response == NULL || memmap_request.response->entries == NULL) {
        bcon_append(kconsole_ptr, "\033[31mcould not load memory map! halting system.", true);
        hcf();
    }

    // ensure hhdm offset
    if (hhdm_request.response == NULL) {
        bcon_append(kconsole_ptr, "\033[31mcould not fetch higher half direct map offset! halting system.", true);
        hcf();

    }

    const struct limine_memmap_response* memmap_response = memmap_request.response;
    const struct limine_hhdm_response* hhdm_response = hhdm_request.response;
    
    PAGE_FRAME_ALLOCATOR allocator;
    PAGE_FRAME_ALLOCATOR* allocator_ptr = &allocator;
    global_allocator = allocator_ptr;
    pfa_readEfiMemoryMap(allocator_ptr, memmap_response->entry_count, memmap_response->entries, hhdm_response->offset);
    // DO NOT CALL ANY FUNCTIONS THAT USE THE GLOBAL ALLOCATOR ABOVE THIS LINE! THE KERNEL WILL CRASH!
    // below here is safe :)

    PAGE_TABLE* page_map_lv4 = (PAGE_TABLE*)((uint64_t)pfa_requestPage(allocator_ptr) + hhdm_response->offset);
    memset(page_map_lv4, 0, 0x1000);

    PAGE_TABLE_MANAGER page_table_manager;
    PAGE_TABLE_MANAGER* page_table_manager_ptr = &page_table_manager;
    ptm_init(page_table_manager_ptr, page_map_lv4);

    for (uint64_t i = 0; i < getMemorySize(memmap_response->entry_count, memmap_response->entries); i += 0x1000) {
        ptm_mapMemory(page_table_manager_ptr, (void*)i, (void*)i, hhdm_response->offset);
    }

    void* page_map_lv4_physical = (void*)((uint64_t)page_map_lv4 - hhdm_response->offset);

    /* the following code is commented because it breaks! fix it very very soon pls
    // load map lv4 into cr3 so that map works correctly
    asm("mov %0, %%cr3" : : "r" (page_map_lv4_physical));
    
    // remove hhdm offset from things we need to use now that we are identity mapped
    kconsole_ptr = (BASIC_CONSOLE*)((uint64_t)kconsole_ptr - hhdm_response->offset);
    framebuffer = (struct limine_framebuffer*)((uint64_t)framebuffer - hhdm_response->offset);
    font_data = &_binary_assets_fonts_zap_ext_light32_psf_start; */

    bcon_init(kconsole_ptr, framebuffer, font_data); // reinit the console so that it uses the new values

    bcon_write(kconsole_ptr, "\033[32msimplexModus \033[36mv0\n", true);
    bcon_append(kconsole_ptr, "\033[37mframebuffer dimensions: ", true);
    bcon_append(kconsole_ptr, fb_width_str, true);
    bcon_append(kconsole_ptr, "x", true);
    bcon_append(kconsole_ptr, fb_height_str, true);
    bcon_append(kconsole_ptr, "\n", true);

    bcon_append(kconsole_ptr, "total memory size: \033[36m", true);
    bcon_append(kconsole_ptr, itoa64(getMemorySize(memmap_response->entry_count, memmap_response->entries) / 1024, 10), true);
    bcon_append(kconsole_ptr, " KiB\033[37m\n", true);

    bcon_append(kconsole_ptr, "free memory: \033[36m", true);
    bcon_append(kconsole_ptr, itoa64(pfa_getFreeMemory() / 1024, 10), true);
    bcon_append(kconsole_ptr, " KiB\033[37m\n", true);

    bcon_append(kconsole_ptr, "used memory: \033[36m", true);
    bcon_append(kconsole_ptr, itoa64(pfa_getUsedMemory() / 1024, 10), true);
    bcon_append(kconsole_ptr, " KiB\033[37m\n", true);

    bcon_append(kconsole_ptr, "reserved memory: \033[36m", true);
    bcon_append(kconsole_ptr, itoa64(pfa_getReservedMemory() / 1024, 10), true);
    bcon_append(kconsole_ptr, " KiB\033[37m\n", true);

    // done, hang
    hcf();
}
