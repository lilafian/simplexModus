#include "psfFonts.h"

void drawChar(int x, int y, char c, uint32_t color, const void* font_data, struct limine_framebuffer *framebuffer) {
    const PSF2_header* header = (const PSF2_header*)font_data;
    if (header->magic != PSF2_MAGIC) {
        return; // Invalid font
    }

    const unsigned char* glyph = (const unsigned char*)font_data + header->headersize + (c * header->bytesperglyph);

    for (uint32_t yy = 0; yy < header->height; yy++) {
        for (uint32_t xx = 0; xx < header->width; xx++) {
            int bit = yy * header->width + xx;
            int byte = bit / 8;
            if ((glyph[byte] & (1 << (7 - (bit % 8)))) > 0) {
                drawPixel(x + xx, y + yy, framebuffer, color);
            }
        }
    }
}

void drawString(int x, int y, const char* string, uint32_t color, const void* font_data, struct limine_framebuffer *framebuffer) {
    while (*string != '\0') {
        drawChar(x, y, *string, color, font_data, framebuffer);
        x += 16;
        string++;
    }
}
