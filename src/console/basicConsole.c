// Basic console (tty-like) implementation

#include "basicConsole.h"
#include "../cfuncs/cfuncs.h"
#include "../rendering/psfFonts.h"

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
    if (console == NULL || console->framebuffer == NULL) {
        return;
    }
    struct limine_framebuffer* fb = console->framebuffer;
    clearFramebuffer(fb);
    drawString(0, 0, console->out_content, 0xffffffff, console->display_font, fb, false);
}

void bcon_init(BASIC_CONSOLE* console, struct limine_framebuffer* framebuffer, void* display_font) {
    console->framebuffer = framebuffer;
    console->display_font = display_font;
    console->cursorX = 0;
    console->cursorY = 0;
    console->width = framebuffer->width / 16;
    console->height = framebuffer->height / 32;
    console->out_content[0] = '\0';
}
