#ifndef _LEANUX_KEYBOARD_H
#define _LEANUX_KEYBOARD_H

#include <lib/cirqueue.h>

#define KEYBOARD_PORT 0x60


/* special keys */
enum{ KB_NONE=0xE0, KB_ESC, KB_ENTER, KB_CTRL, KB_RIGHT_CTRL, KB_LEFT_SHIFT,
      KB_RIGHT_SHIFT, KB_ALT, KB_RIGHT_ALT, KB_CAPSLOCK, KB_HOME, KB_END,
      KB_INSERT, KB_DEL, KB_PAGE_UP, KB_PAGE_DOWN, KB_LEFT, KB_RIGHT, KB_UP,
      KB_DOWN, KB_BACKSPACE};

struct cirqueue kb_buf;

uint16_t KEYMAP[0x80];

void int_21_keyboard();

void keyboard_init();

#endif

