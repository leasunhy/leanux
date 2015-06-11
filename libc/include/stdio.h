#ifndef _LEANUX_LIBC_STDIO_H
#define _LEANUX_LIBC_STDIO_H

#include <stdarg.h>

int printf(const char *fmt, ...);
int vprintf(const char *fmt, va_list ap);
int sprintf(char *buf, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list ap);
int putchar(int c);
int puts(const char *s);

#endif  // _LEANUX_LIBC_STDIO_H

