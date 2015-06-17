#ifndef _LEANUX_TTY_H
#define _LEANUX_TTY_H

#include <stddef.h>
#include <stdint.h>

void tty_init();
void tty_clear();
void tty_putchar(char c);
size_t tty_read(char *buf, size_t len);
void tty_write(const char *data, size_t len);
void tty_write_colored(const char *data, size_t size, uint8_t color);
void tty_writestring(const char *str);

int tty_getcursor();
void tty_setcursor(int x, int y);
void tty_updatecursor();
void tty_handle_scroll();

#endif

