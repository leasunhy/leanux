#include <stddef.h>

#include <drivers/tty.h>

void shell_main() {
    char buf[64];
    size_t len = tty_read(buf, 60);
}

