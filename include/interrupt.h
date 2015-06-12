#ifndef _LEANUX_INTERRUPT_H
#define _LEANUX_INTERRUPT_H

#include <stdint.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

struct INT_DESC {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint16_t zeros           : 8;
    uint16_t type            : 4;
    uint16_t s               : 1;
    uint16_t dpl             : 2;
    uint16_t present         : 1;
    uint16_t offset_high;
}__attribute__((packed));

extern struct INT_DESC *IDT;

void idt_init();
void pic_init();
void send_pic_eoi(uint8_t irq);
void enable_irq(uint8_t irq);

void register_interrupt(uint8_t irq, uint16_t selector, uint32_t offset);

struct INT_DESC make_int_desc(uint16_t selector, uint32_t offset);

inline void disable_int() {
    __asm__ __volatile__("cli");
}

inline void enable_int() {
    __asm__ __volatile__("sti");
}

void int_null();
void int_null_0();
void int_null_1();

#endif

