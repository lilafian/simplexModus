// simplexModus

// includes
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "boot/limine.h"
#include "cfuncs/cfuncs.h"
#include "rendering/psfFonts.h"
#include "console/basicConsole.h"

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

// TEMPORARY FUNCTION FOR INTEGER CONVERSION!!! PLEASE MAKE A BETTER ONE AT SOME POINT!!!

char* itoa(int val, int base){

    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

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

    const void* font_data = &_binary_assets_fonts_zap_ext_light32_psf_start;

    /* const char* fb_width_str = itoa(framebuffer->width, 10);
    const char* fb_height_str = itoa(framebuffer->height, 10); */

    BASIC_CONSOLE kconsole;
    BASIC_CONSOLE* kconsole_ptr = &kconsole;
    bcon_init(kconsole_ptr, framebuffer, font_data);

    /* drawString(0, 0, "simplexModus\nv0", 0x00FF00, font_data, framebuffer);
    drawString(0, 64, "framebuffer resolution: ", 0xFFFFFF, font_data, framebuffer);
    drawString(384, 64, fb_width_str, 0xFFFFFF, font_data, framebuffer);
    drawString(464, 64, fb_height_str, 0xFFFFFF, font_data, framebuffer); */

    bcon_write(kconsole_ptr, "simplexModus v0", true);

    // done, hang
    hcf();
}
