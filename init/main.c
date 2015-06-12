#include <leanux.h>
#include <tty.h>
#include <interrupt.h>

void kernel_main() {
    tty_init();
    pic_init();
    idt_init();

    for (int i = 0; i < 30; ++i)
        tty_write("haha\n", 5);
    tty_write("####\n", 5);
}

