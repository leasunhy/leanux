#include <stddef.h>
#include <stdint.h>

#include <low_level.h>
#include <interrupt.h>

struct INT_DESC *IDT;

void idt_init() {
    struct INT_DESC null_handler = make_int_desc(0x8, (uint32_t) &int_null);
    for (int i = 0; i < 256; ++i)
        IDT[i] = null_handler;
    __asm__ __volatile__("lidt [idtr]");
}

struct INT_DESC make_int_desc(uint16_t selector, uint32_t offset) {
    struct INT_DESC res;
    res.offset_low = offset & 0xFFFF;
    res.segment_selector = selector;
    res.type = 0xE;  /* 32-bit interrupt gate */
    res.s = 0;          /* fixed to 0 for int gates */
    res.dpl = 0;        /* highest priv. */
    res.present = 1;
    res.offset_high = offset >> 16;
    return res;
}

void pic_init() {
    port_byte_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    port_byte_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    /* remap ints to 0x20..0x2f */
    port_byte_out(PIC1_DATA, 0x20);
    io_wait();
    port_byte_out(PIC2_DATA, 0x28);
    io_wait();

    /* set master and slave */
    port_byte_out(PIC1_DATA, 4);
    io_wait();
    port_byte_out(PIC2_DATA, 2);
    io_wait();

    /* set master and slave */
    port_byte_out(PIC1_DATA, ICW4_8086);
    io_wait();
    port_byte_out(PIC2_DATA, ICW4_8086);
    io_wait();

    /* mask all ints for now */
    port_byte_out(PIC1_DATA, 0xff);
    io_wait();
    port_byte_out(PIC2_DATA, 0xff);
    io_wait();

    struct INT_DESC null_int = make_int_desc(0x8, (uint32_t)&int_null_0);
    for (int i = 0x20; i <= 0x28; ++i)
        IDT[i] = null_int;
    null_int = make_int_desc(0x8, (uint32_t)&int_null_1);
    for (int i = 0x29; i <= 0x2f; ++i)
        IDT[i] = null_int;
}

void send_pic_eoi(uint8_t irq) {
    if (irq >= 8)
        port_byte_out(PIC2_COMMAND, 0x20);
    port_byte_out(PIC1_COMMAND, 0x20);
}

void enable_irq(uint8_t irq) {
    uint8_t mask;
    if (irq >= 8) {
        mask = port_byte_in(PIC2_DATA);
        mask &= ~(1 << (irq - 8));
        port_byte_out(PIC2_DATA, mask);
    } else {
        mask = port_byte_in(PIC1_DATA);
        mask &= ~(0x1 << irq);
        port_byte_out(PIC1_DATA, mask);
    }
}

