#include <stdint.h>

#include <leanux.h>
#include <tty.h>
#include <interrupt.h>

#include <stdio.h>

void _keyboard() {
    uint8_t val = port_byte_in(0x60);
    printk("haha\n");
    send_pic_eoi(0x21);
    enable_irq(0x21);
}

void keyboard();

void kernel_main() {
    disable_interrupt();
    tty_init();
    idt_init();
    pic_init();

    register_interrupt(0x21, 0x8, (uint32_t)&keyboard);

    enable_interrupt();

    int i = 0;
    while (1) {
        ++i;
    }
}

