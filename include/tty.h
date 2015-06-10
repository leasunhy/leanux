#ifndef _LEANUX_TTY_H
#define _LEANUX_TTY_H

#include <stddef.h>

void tty_init();
void tty_putchar(char c);
void tty_write(const char *data, size_t len);
void tty_writestring(const char *str);

#endif

