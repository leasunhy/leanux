#ifndef _LEANUX_LIBC_STDLIB_H
#define _LEANUX_LIBC_STDLIB_H

#include <stddef.h>

int atoi(const char *nptr);

int printf(const char *fmt, ...);
int snprintf(char *buf, size_t size, const char *fmt, ...);

int vprintf(const char *fmt, va_list ap);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);

#endif

