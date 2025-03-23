// Basic console (tty-like) struct/methods

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../rendering/psfFonts.h"
#include "../boot/limine.h"

typedef struct {
    char out_content[1024];
    uint16_t width;
    uint16_t height;
    int cursorX;
    int cursorY;
    struct limine_framebuffer* framebuffer;
    void* display_font;
} BASIC_CONSOLE;

void bcon_write(BASIC_CONSOLE* console, const char* text, bool update_display);
void bcon_append(BASIC_CONSOLE* console, const char* text, bool update_display);
void bcon_display(BASIC_CONSOLE* console);
void bcon_init(BASIC_CONSOLE* console, struct limine_framebuffer* framebuffer, void* display_font);

