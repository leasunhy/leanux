#include <stdio.h>

#include <leanux/sys.h>

int getch() {
    char c[2];
    read(0, c, 1);
    return c[0];
}

