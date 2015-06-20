#include <stdint.h>

#include <leanux/leanux.h>
#include <leanux/interrupt.h>
#include <leanux/syscall.h>
#include <leanux/low_level.h>
#include <leanux/mm.h>
#include <drivers/tty.h>
#include <drivers/keyboard.h>
#include <lib/printk.h>

void shell_main();

void kernel_main(void *multiboot_info) {
    disable_interrupt();
    tty_init();
    idt_init();
    pic_init();
    page_init();
    mm_init(((uint32_t*)multiboot_info)[2]);

    keyboard_init();

    register_interrupt(0x80, 0x8, (uint32_t)&syscall);

    enable_interrupt();

    while (1) {
        shell_main();
    }
}

