/*
 * simplexModus/src/kernel/common/cfuncs.c
 * copyright (c) 2025 lilaf, GNU General Public License v3.0.
 * view more license information by viewing simplexModus/LICENSE.
 *
 * C standard, common, and/or convienient function implementations
 */

#include "cfuncs.h"
#include <stdint.h>

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *start, uint8_t value, uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        *(uint8_t*)((uint64_t)start + 1) = value;
    }

    return start;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

char* itoa(int val, int base) {
    if (val == 0) return "0";

    static char buf[64] = {0};

    int i = 62;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];
}

char* itoa64(uint64_t val, int base) {
    if (val == 0) return "0";
    
    static char buf[64] = {0};

    int i = 62;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];
}

// custom implementations of normal stdlib funcs will be prepended with "smk_"

int smk_strlen(const char* text) {
    int length = 0;
    while (text[length] != '\0') {
        length++;
    }
    return length;
}

void smk_concat(char* dest, const char* src) {
    while (*dest != '\0') {
        dest++;
    }

    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';
}

void smk_concat_char(char* dest, const char src) {
    while (*dest != '\0') {
        dest++;
    }

    *dest = src;
    dest++;

    *dest = '\0';
}

bool smk_isdigit(char character) {
    if (character >= '0' && character <= '9') return true;
    return false;
}

int smk_occurrences(char string[], char character) {
    int count = 0;
    for (int i = 0; i < smk_strlen(string); i++) {
        if (string[i] == character) {
            count++;
        }
    }
    return count;
}

// change the max_token and max_token_len if you want more tokens, also this might be changed at some point

#define MAX_TOKENS 64
#define MAX_TOKEN_LEN 512

char** smk_strtok(const char* str, char delim) {
    static char* tokens[MAX_TOKENS + 1];
    static char token_storage[MAX_TOKENS][MAX_TOKEN_LEN];
    
    int token_count = 0;
    int char_pos = 0;
    
    for (int i = 0; i < MAX_TOKENS; i++) {
        tokens[i] = NULL;
        token_storage[i][0] = ' ';
    }
    
    while (*str && token_count < MAX_TOKENS) {
        if (*str == delim) {
            token_storage[token_count][char_pos] = ' ';
            tokens[token_count] = token_storage[token_count];
            token_count++;
            char_pos = 0;
            while (*(str+1) == delim) str++;
        } else {
            if (char_pos < MAX_TOKEN_LEN - 1) {
                token_storage[token_count][char_pos++] = *str;
            }
        }
        str++;
    }
    
    if (char_pos > 0 && token_count < MAX_TOKENS) {
        token_storage[token_count][char_pos] = ' ';
        tokens[token_count++] = token_storage[token_count];
    }
    
    tokens[token_count] = NULL;
    return tokens;
}
