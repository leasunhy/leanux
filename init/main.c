#include <tty.h>

void kernel_main() {
    tty_init();
    tty_write("haha", 4);
}

