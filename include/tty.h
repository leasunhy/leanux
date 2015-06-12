#ifndef _LEANUX_TTY_H
#define _LEANUX_TTY_H

#include <stddef.h>

void tty_init();
void tty_clear();
void tty_putchar(char c);
void tty_write(const char *data, size_t len);
void tty_writestring(const char *str);

int tty_getcursor();
void tty_setcursor(int x, int y);
void tty_updatecursor();
void tty_handle_scroll();

#endif

