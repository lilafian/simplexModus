/*
 * simplexModus/src/console/basicConsole/basicConsole.h
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * basic tty-like console struct and function declarations
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../../graphics/psfFonts.h"
#include "../../boot/limine.h"

typedef struct {
    char out_content[4096];
    uint16_t width;
    uint16_t height;
    uint32_t color;
    int cursorX;
    int cursorY;
    struct limine_framebuffer* framebuffer;
    void* display_font;
    char* name;
} BASIC_CONSOLE;

void bcon_write(BASIC_CONSOLE* console, const char* text, bool update_display);
void bcon_append(BASIC_CONSOLE* console, const char* text, bool update_display);
void bcon_display(BASIC_CONSOLE* console);
void bcon_init(BASIC_CONSOLE* console, struct limine_framebuffer* framebuffer, void* display_font, char* name);
