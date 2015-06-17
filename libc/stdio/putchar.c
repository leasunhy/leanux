#include <stdio.h>

#include <drivers/tty.h>

int putchar(int c) {
    char cc = c;
    tty_write(&cc, sizeof(c));
    return c;
}

