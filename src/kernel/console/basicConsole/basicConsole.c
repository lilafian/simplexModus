/*
 * simplexModus/src/kernel/console/basicConsole/basicConsole.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * basic tty-like console function implementations
 */

#include "basicConsole.h"
#include "../../common/cfuncs.h"
#include "../../graphics/psfFonts.h"

void bcon_write(BASIC_CONSOLE* console, const char* text, bool update_display) {
    for (int i = 0; i < smk_strlen(text) && i < 1024; i++) {
        console->out_content[i] = text[i];
    }

    if (update_display) {
        bcon_display(console);
    }
}


void bcon_append(BASIC_CONSOLE* console, const char* text, bool update_display) {
    if (smk_strlen(text) + smk_strlen(console->out_content) > 1023) { // 1024 minus null terminator
        return;
    }

    smk_concat(console->out_content, text);

    if (update_display) {
        bcon_display(console);
    }
}

void bcon_display(BASIC_CONSOLE* console) {
    if (!console || !console->framebuffer) return;
    
    clearFramebuffer(console->framebuffer);

    int total_lines = smk_occurrences(console->out_content, '\n') + 1;
    
    const char *start = console->out_content;
    if (total_lines > console->height) {
        int lines_to_skip = total_lines - console->height;
        while (lines_to_skip > 0 && *start) {
            if (*start++ == '\n') {
                lines_to_skip--;
            }
        }
    }

    drawString(0, 16, start, 0xffffffff, console->display_font, console->framebuffer, false);

    drawRect(0, 0, console->framebuffer->width, 16, console->framebuffer, 0xbbbbbb);
    drawString(0, 0, "simplexModus v0", 0x000000, console->display_font, console->framebuffer, false);
    int nameX = console->framebuffer->width - smk_strlen(console->name) * 8;
    drawString(nameX, 0, console->name, 0x000000, console->display_font, console->framebuffer, false);
}


void bcon_init(BASIC_CONSOLE* console, struct limine_framebuffer* framebuffer, void* display_font, char* name) {
    console->framebuffer = framebuffer;
    console->display_font = display_font;
    console->cursorX = 0;
    console->cursorY = 0;
    console->width = framebuffer->width / 8;
    console->height = framebuffer->height / 16 - 1;
    console->out_content[0] = '\0';
    console->name = name;
}
