#include <stddef.h>
#include <string.h>

int atoi(const char *nptr) {
    int i = 0;
    int sign = 1;
    char c = *nptr;
    if (c == '-') {
        sign = -1;
        ++nptr;
    }
    while ((c = *nptr++) != '\0')
        i = i * 10 + (c - '0');
    i *= sign;
    return i;
}

