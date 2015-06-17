#ifndef _LEANUX_TTY_H
#define _LEANUX_TTY_H

#include <stddef.h>
#include <stdint.h>

void tty_init();
void tty_clear();
void tty_putchar(char c);
void tty_putchar_ntimes(char c, uint8_t color, size_t n);
size_t tty_read(char *buf, size_t len);
void tty_write(const char *data, size_t len);
void tty_write_colored(const char *data, size_t size, uint8_t color);
void tty_writestring(const char *str);
void tty_writestring_colored(const char *str, uint8_t color);

int tty_getcursor();
void tty_setcursor(int x, int y);
void tty_updatecursor();
void tty_handle_scroll();

#endif

