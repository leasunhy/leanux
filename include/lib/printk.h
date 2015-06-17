#ifndef _LEANUX_PRINTK_H
#define _LEANUX_PRINTK_H

#include <stddef.h>
#include <stdarg.h>

void kputchar(int c);

int printk(const char *fmt, ...);

#endif

