#include <stdint.h>

#include <leanux/leanux.h>
#include <leanux/interrupt.h>
#include <leanux/low_level.h>
#include <drivers/tty.h>
#include <drivers/keyboard.h>
#include <lib/printk.h>

void shell_main();

void kernel_main() {
    disable_interrupt();
    tty_init();
    idt_init();
    pic_init();

    keyboard_init();

    enable_interrupt();

    while (1) {
        shell_main();
    }
}

