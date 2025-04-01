#pragma once
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.l
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
//  - osdev wiki

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *start, uint8_t value, uint64_t count);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

char* itoa(int val, int base);
char* itoa64(uint64_t val, int base);

// custom implementations of normal stdlib funcs (or other simple util funcs) will be prepended with "smk_"

int smk_strlen(const char* text);
void smk_concat(char* dest, const char* src);
bool smk_isdigit(char character);
int smk_occurrences(char string[], char character);
char** smk_strtok(const char* string, char delimiter);
