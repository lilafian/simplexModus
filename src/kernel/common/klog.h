/*
 * simplexModus/src/common/klog.h
 * copyright (c) 2025 lilaf, MIT license.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * universal kernel logging function and variable declarations
 */

#pragma once

#include "../console/basicConsole/basicConsole.h"
#include <stdarg.h>

#define KERNEL_LOG_MODE_CON_DISPLAYED 0
#define KERNEL_LOG_MODE_CON_NODISPLAY 1
#define KERNEL_LOG_MODE_FILE_ONLY 2 // future reserved
#define KERNEL_LOG_MODE_FILE_CON_DISPLAYED 3 // future reserved
#define KERNEL_LOG_MODE_FILE_CON_NODISPLAY 4 // future reserved

extern uint8_t kernel_log_mode;
extern BASIC_CONSOLE* kernel_log_console;
extern char* kernel_log_file_path; // future reserved

void klog(char* message);
void klogf(char* fmt, ...);

void klog_init(uint8_t mode, BASIC_CONSOLE* console, char* file_path);
