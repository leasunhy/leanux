#include <stddef.h>
#include <stdarg.h>

#include <stdio.h>

int sprintf(char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int char_count = vsnprintf(buf, size, fmt, args);
    va_end(args);
    return char_count;
}

int vsprintf(char *buf, const char *fmt, ...) {
    /*
     *TODO: implement
     */
}

