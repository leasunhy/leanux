#include <string.h>

char *strncpy(char *dest, const char *src, size_t n) {
    size_t count = 0;
    char *p = dest;
    while (*src && count++ != n - 1)
        *p++ = *src++;
    *p = '\0';
    return dest;
}

