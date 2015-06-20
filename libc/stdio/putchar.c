#include <stdio.h>

#include <leanux/sys.h>

int putchar(int c) {
    s_putchar(c);
    return c;
}

