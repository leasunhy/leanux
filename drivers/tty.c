#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <leanux/interrupt.h>
#include <leanux/low_level.h>
#include <drivers/keyboard.h>
#include <drivers/tty.h>
#include <drivers/vga.h>
#include <lib/utils.h>
#include <lib/cirqueue.h>

enum {
    REG_SCREEN_CTRL = 0x3D4,
    REG_SCREEN_DATA = 0x3D5
};

static size_t strlen(const char *s) {
    size_t len = 0;
    while (*s) {
        ++len;
        ++s;
    }
    return len;
}

int tty_y;
int tty_x;
uint8_t tty_color;
uint16_t *tty_buffer;

void tty_init() {
    tty_y = 0;
    tty_x = 0;
    tty_updatecursor();
    tty_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    tty_buffer = VGA_MEMORY;
    tty_clear();
}

void tty_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        tty_buffer[i] = make_vgaentry(' ', tty_color);
}

void tty_setcolor(uint8_t color) {
    tty_color = color;
}

int tty_getoffset(int x, int y) {
    const int index = y * VGA_WIDTH + x;
    return index;
}

void tty_put_entry_at(char c, uint8_t color, int x, int y) {
    tty_buffer[tty_getoffset(x, y)] = make_vgaentry(c, color);
}

void tty_putchar(char c) {
    if (c == '\r') {
        tty_x = 0;
    } else if (c == '\b' && (tty_x + tty_y != 0)) {
        if (tty_x == 0) {
            tty_x = VGA_WIDTH - 1;
            tty_y -= 1;
        } else {
            tty_x -= 1;
        }
        tty_put_entry_at(' ', tty_color, tty_x, tty_y);
    } else if (c == '\n') {
        tty_x = 0;
        ++tty_y;
    } else {
        tty_put_entry_at(c, tty_color, tty_x, tty_y);
        if (++tty_x == VGA_WIDTH) {
            tty_x = 0;
            ++tty_y;
        }
    }
    tty_handle_scroll();
    tty_updatecursor();
}

void tty_putchar_ntimes(char c, uint8_t color, size_t n) {
    uint8_t ori_color = tty_color;
    tty_color = color;
    for (size_t i = 0; i < n; ++i)
        tty_putchar(c);
    tty_color = ori_color;
}

void tty_handle_scroll() {
    if (tty_y < VGA_HEIGHT)
        return;
    int last_row_offset = (VGA_HEIGHT - 1) * VGA_WIDTH;
    memory_copy((uint8_t*)VGA_MEMORY,
            (uint8_t*)VGA_MEMORY + VGA_WIDTH * 2, last_row_offset * 2);
    for (int i = 0; i < VGA_WIDTH; ++i)
        VGA_MEMORY[last_row_offset + i] = make_vgaentry(' ', tty_color);
    --tty_y;
}

size_t tty_read(char *buf, size_t len) {
    size_t i = 0;
    while (i != len) {
        uint16_t scan_code;
        while (!cirqueue_serve(&kb_buf, &scan_code))
            /*nothing here*/;
        uint16_t key = KEYMAP[scan_code & 0xff];
        if ((scan_code & 0xFF) < 0xE0) {  /* it's a normal key */
            if (key == KB_ENTER) {
                tty_putchar('\n');
                break;
            } else if (key == KB_BACKSPACE) {
                if (i != 0) {
                    tty_putchar('\b');
                    i -= 1;
                }
            } else {
                buf[i] = key & 0xFF;
                tty_putchar(buf[i]);
                i += 1;
            }
        } else {  /* extended key */

        }
    }
    buf[i] = '\0';
    return i;
}

void tty_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; ++i)
        tty_putchar(data[i]);
}

void tty_write_colored(const char *data, size_t size, uint8_t color) {
    uint8_t ori_color = tty_color;
    tty_color = color;
    tty_write(data, size);
    tty_color = ori_color;
}

void tty_writestring(const char *str) {
    tty_write(str, strlen(str));
}

void tty_writestring_colored(const char *str, uint8_t color) {
    tty_write_colored(str, strlen(str), color);
}

void tty_writestring_at(const char *str, uint8_t color, uint32_t row, uint32_t col) {
    while (*str) {
        tty_put_entry_at(*str, color, row, col);
        str++;
    }
}

int tty_getcursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset;
}

void tty_setcursor(int x, int y) {
    int offset = tty_getoffset(x, y);
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xFF));
}

void tty_updatecursor() {
    tty_setcursor(tty_x, tty_y);
}

