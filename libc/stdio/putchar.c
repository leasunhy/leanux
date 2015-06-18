#include <stdio.h>

#include <leanux/sys.h>

int putchar(int c) {
    char cc[] = {c, '\0'};
    write(1, (const char *)&cc, 0x0F);
    return c;
}

