#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <vga.h>

static size_t strlen(const char *s) {
    size_t len = 0;
    while (*s) {
        ++len;
        ++s;
    }
    return len;
}

size_t tty_y;
size_t tty_x;
uint8_t tty_color;
uint16_t *tty_buffer;

void tty_init() {
    tty_y = 0;
    tty_x = 0;
    tty_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    tty_buffer = VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
        tty_buffer[i] = make_vgaentry(' ', tty_color);
    }
}

void tty_setcolor(uint8_t color) {
    tty_color = color;
}

size_t tty_getoffset(size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    return index;
}


void tty_put_entry_at(char c, uint8_t color, size_t x, size_t y) {
    tty_buffer[tty_getoffset(x, y)] = make_vgaentry(c, color);
}

void tty_putchar(char c) {
    if (c == '\r') {
        tty_x = 0;
    } else if (c == '\b') {
        if (tty_x == 0) {
            tty_x = VGA_WIDTH - 1;
            tty_y -= 1;
        } else {
            tty_x -= 1;
        }
        tty_put_entry_at(' ', tty_color, tty_x, tty_y);
    } else if (c == '\n') {
        tty_x = 0;
        if (++tty_y == VGA_HEIGHT)
            tty_y = 0;
    } else {
        tty_put_entry_at(c, tty_color, tty_x, tty_y);
        if (++tty_x == VGA_WIDTH) {
            tty_x = 0;
            if (++tty_y == VGA_HEIGHT) {
                tty_y = 0;
            }
        }
    }
    tty_put_entry_at('_', tty_color, tty_x, tty_y);
}

void tty_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; ++i)
        tty_putchar(data[i]);
}

void tty_writestring(const char *str) {
    tty_write(str, strlen(str));
}

