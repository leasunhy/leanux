#include <stdio.h>

#include <leanux/sys.h>

int putchar(int c) {
    char cc = (char)c;
    s_putchar(c);
    return c;
}

