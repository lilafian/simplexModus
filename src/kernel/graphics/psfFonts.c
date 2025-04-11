/*
 * simplexModus/src/kernel/graphics/psfFonts.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * framebuffer pc screen font renderer function implementations
 */

#include "psfFonts.h"
#include "../common/cfuncs.h"
#include "colors/colors.h"

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

void drawString(int x, int y, const char* string, uint32_t color, const void* font_data, struct limine_framebuffer *framebuffer, bool no_color) {
    uint32_t drawn_color = color; // use drawn_color to account for ansi codes

    while (*string != '\0') {
        if (*string == '\n') {
            y += 16;
            x = 0;
        } else {
            if (!no_color && *string == '\033' && *(string + 1) == '[') { // is ansi color code escape sequence
                string += 2; // now at character after [

                uint16_t ansi_buffer = 0;
                while (smk_isdigit(*string)) {
                    // this is probably insecure but who cares right?
                    ansi_buffer = (ansi_buffer * 10) + (*string - '0');
                    string++;
                }
                drawn_color = getHexFromANSI(ansi_buffer);
            } else {
                drawChar(x, y, *string, drawn_color, font_data, framebuffer);
                x += 8;
            }
        }
        string++;
    }
}
