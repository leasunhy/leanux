#include <leanux/interrupt.h>
#include <leanux/low_level.h>

#include <drivers/keyboard.h>

#include <lib/cirqueue.h>

uint16_t KEYMAP[0x80] = {
    KB_NONE, KB_ESC, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', KB_BACKSPACE, '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', KB_ENTER, KB_CTRL, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', KB_LEFT_SHIFT, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', KB_RIGHT_SHIFT, KB_NONE,
    KB_ALT, ' ', KB_CAPSLOCK, KB_NONE, KB_NONE, KB_NONE, KB_NONE, KB_NONE,
    KB_NONE, KB_NONE, KB_NONE, KB_NONE, KB_NONE, KB_NONE, KB_NONE, KB_HOME,
    KB_UP, KB_PAGE_UP, '-', KB_LEFT, '5', KB_RIGHT, '+', KB_END,
    KB_NONE, KB_PAGE_DOWN, KB_INSERT, KB_DEL
};

struct cirqueue kb_buf;
uint8_t kb_status;

void keyboard_init() {
    register_interrupt(0x21, 0x8, (uint32_t)&int_21_keyboard);
    cirqueue_init(&kb_buf);
    kb_status = 0;
}

void _int_21_keyboard() {
    disable_interrupt();
    uint8_t val = port_byte_in(KEYBOARD_PORT);
    /* TODO: handle extended keys(val >= 0xE0) as well */
    if (val >= 0xE0)
        val = port_byte_in(KEYBOARD_PORT);
    if (!(val & 0x80)) {  /* key press event */
        cirqueue_enqueue(&kb_buf, val | (kb_status << 8));
    }

    send_pic_eoi(0x21);
    enable_interrupt();
}

