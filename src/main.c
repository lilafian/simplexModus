// simplexModus

// includes
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "boot/limine.h"
// #include "rendering/fbPixRenderer.h"
#include "rendering/psfFonts.h"

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

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.l
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
//  - osdev wiki

inline void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

inline void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

inline void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

inline int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

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

    const char* fbWidthStr = itoa(framebuffer->width, 10);
    const char* fbHeightStr = itoa(framebuffer->height, 10);

    drawString(0, 0, "simplexModus v0", 0x00FF00, font_data, framebuffer);
    drawString(0, 32, "framebuffer resolution: ", 0xFFFFFF, font_data, framebuffer);
    drawString(384, 32, fbWidthStr, 0xFFFFFF, font_data, framebuffer);
    drawString(464, 32, fbHeightStr, 0xFFFFFF, font_data, framebuffer);

    // done, hang
    hcf();
}
