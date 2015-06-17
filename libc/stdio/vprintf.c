#include <stddef.h>
#include <stdarg.h>

#include <stdio.h>

#include <drivers/tty.h>

static size_t itoa(char *buf, int i, int base) {
    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char tmp[40];
    char *ptr = tmp;
    size_t count = 0;

    if (i == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return 1;
    }
    if (i < 0) {
        *buf++ = '-';
        i = -i;
        count++;
    }
    while (i != 0) {
        *ptr++ = digits[i % base];
        i /= base;
        ++count;
    }
    while (ptr != tmp)
        *buf++ = *--ptr;
    *buf = '\0';
    return count;
}

int printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int char_count = vprintf(fmt, args);
    va_end(args);
    return char_count;
}

int vprintf(const char *fmt, va_list ap)
{
    int char_count = 0;
    char c;
    while ((c = *fmt++) != '\0') {
        if (c != '%') {
            putchar(c);
            continue;
        }
        c = *fmt;
        if (c == 'c') {
            putchar(va_arg(ap, int));
        } else if (c == 'd') {
            char num_buf[40];
            itoa(num_buf, va_arg(ap, int), 10);
            tty_writestring(num_buf);
        } else if (c == 's') {
            tty_writestring(va_arg(ap, char *));
        }
    }
    ++fmt;
    return char_count;
}

