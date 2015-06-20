#include <stdio.h>

#include <leanux/sys.h>

char *gets(char *s) {
    read(0, s, 128);
    return s;
}

