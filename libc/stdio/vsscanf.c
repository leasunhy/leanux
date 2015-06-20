#include <stdarg.h>
#include <stddef.h>

#include <stdio.h>

#include <leanux/sys.h>

int scanf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int res = vscanf(fmt, ap);
    va_end(ap);
    return res;
}

int vscanf(const char *fmt, va_list ap) {
    char buf[LIBC_SCANF_BUFFER_SIZE];
    read(0, buf, LIBC_SCANF_BUFFER_SIZE);
    return vsscanf(buf, fmt, ap);
}

int sscanf(const char *buf, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int res = vsscanf(buf, fmt, ap);
    va_end(ap);
    return res;
}

static const char *number(const char *buf, int *pres) {
    int sign = 1;
    char c = *buf;
    if (c == '-') {
        sign = -1;
        buf++;
    }
    *pres = 0;
    while ((c = *buf) != '\0') {
        if (c < '0' || c > '9')
            break;
        *pres *= 10;
        *pres += c - '0';
        ++buf;
    }
    *pres *= sign;
    return buf;
}

int vsscanf(const char *buf, const char *fmt, va_list ap) {
    int item_count = 0;
    char bc, fc;
    while ((bc = *buf) != '\0' && (fc = *fmt) != '\0') {
        if (fc != '%') {
            if (fc != bc)
                return item_count;
            else
                ++buf;
        } else {
            if ((fc = *++fmt) == '\0')
                return item_count;
            if (fc == 'c') {
                *va_arg(ap, char *) = *buf++;
            } else if (fc == 'd') {
                buf = number(buf, va_arg(ap, int *));
            }
            ++item_count;
        }
        ++fmt;
    }
    return item_count;
}


