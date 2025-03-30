// color function implementations

#include "colors.h"

uint32_t getHexFromANSI(uint16_t ansi_code) {
    switch (ansi_code) {
        case 30: return 0x000000; // black
        case 31: return 0xFF0000; // red
        case 32: return 0x00FF00; // green
        case 33: return 0xFFFF00; // yellow
        case 34: return 0x0000FF; // blue
        case 35: return 0xFF00FF; // purple / magenta
        case 36: return 0x00FFFF; // cyan
        case 37: return 0xBBBBBB; // light gray
        case 90: return 0x555555; // gray (light black)
        case 91: return 0xFF5555; // light red
        case 92: return 0x55FF55; // light green
        case 93: return 0xFFFF55; // light yellow
        case 94: return 0x5555FF; // light blue
        case 95: return 0xFF55FF; // light purple / magenta
        case 96: return 0x55FFFF; // light cyan
        case 97: return 0xFFFFFF; // white (light light gray)
        default: return 0xFFFFFFF; // Return white if no match is found
    }
}

