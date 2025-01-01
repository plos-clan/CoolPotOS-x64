#include "krlibc.h"

void *memcpy(void *dest, const void *src, size_t n) {
    char *d = (char *) dest;
    const char *s = (const char *) src;

    void *ret = dest;

    if (n < 8) {
        while (n--) {
            *d++ = *s++;
        }
        return ret;
    }

    size_t align = (size_t) d & (sizeof(size_t) - 1);
    if (align) {
        align = sizeof(size_t) - align;
        n -= align;
        while (align--) {
            *d++ = *s++;
        }
    }

    size_t *dw = (size_t *) d;
    const size_t *sw = (const size_t *) s;
    for (size_t i = 0; i < n / sizeof(size_t); i++) {
        *dw++ = *sw++;
    }

    d = (char *) dw;
    s = (const char *) sw;
    size_t remain = n & (sizeof(size_t) - 1);
    while (remain--) {
        *d++ = *s++;
    }

    return ret;
}

size_t strlen(const char *str) {
    const char *s = str;
    while (*s) {
        s++;
    }
    return s - str;
}

char* strcat(char* dest, const char* src) {
    char* ret = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return ret;
}
