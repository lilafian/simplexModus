/*
 * simplexModus/src/kernel/common/klog.c
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * universal kernel logging function definitions
 */

#include "klog.h"
#include "cfuncs.h"
#include "../console/basicConsole/basicConsole.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

extern uint8_t kernel_log_mode = 0;
extern BASIC_CONSOLE* kernel_log_console = 0;
extern char* kernel_log_file_path = ""; // future reserved

void klog(char* message) {
    switch (kernel_log_mode) {
        case KERNEL_LOG_MODE_CON_DISPLAYED:
            bcon_append(kernel_log_console, message, true);
            break;
        case KERNEL_LOG_MODE_CON_NODISPLAY:
            bcon_append(kernel_log_console, message, false);
            break;
        default:
            bcon_append(kernel_log_console, message, true);
            break;
    }
}

void klogf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    buffer[0] = '\0';

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch(*fmt) {
                case 's': { // string
                    char* string = va_arg(args, char*);
                    smk_concat(buffer, string);
                    break;
                }
                case 'd': { // decimal uint
                    uint64_t value = va_arg(args, uint64_t);
                    smk_concat(buffer, itoa64(value, 10));
                    break;
                }
                case 'x': {// hex uint
                    uint64_t value = va_arg(args, uint64_t);
                    smk_concat(buffer, itoa64(value, 16));
                    break;
                }
                case '%': {
                    smk_concat(buffer, "%");
                    break;
                }
                default: {
                    smk_concat(buffer, "%?");
                    break;
                }
            }
        } else {
            char temp[2] = {*fmt, '\0'};
            smk_concat(buffer, temp);
        }
        fmt++;
    }

    va_end(args);

    klog(buffer);
}

void klog_init(uint8_t mode, BASIC_CONSOLE* console, char* file_path) {
    kernel_log_mode = mode;
    kernel_log_console = console;
    kernel_log_file_path = file_path;
}
