#include <stddef.h>
#include <stdarg.h>

#include <lib/utils.h>
#include <drivers/tty.h>

void kputchar(int c) {
    char cc = (char)c;
    tty_write(&cc, 1);
}

int printk(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int char_count = 0;
    char c;
    while ((c = *fmt++) != '\0') {
        if (c != '%') {
            kputchar(c);
            continue;
        }
        c = *fmt;
        if (c == 'c') {
            kputchar(va_arg(ap, int));
        } else if (c == 'd') {
            char num_buf[40];
            kitoa(num_buf, va_arg(ap, int), 10);
            tty_writestring(num_buf);
        } else if (c == 'x') {
            char num_buf[40];
            kitoa(num_buf, va_arg(ap, int), 16);
            tty_writestring(num_buf);
        } else if (c == 's') {
            tty_writestring(va_arg(ap, char *));
        }
        ++fmt;
    }
    va_end(ap);
    return char_count;
}

