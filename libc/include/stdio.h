#ifndef _LEANUX_LIBC_STDIO_H
#define _LEANUX_LIBC_STDIO_H

#include <stdarg.h>

#define LIBC_SCANF_BUFFER_SIZE 128
#define LIBC_GETS_MAX_LEN 128

int putchar(int c);
int puts(const char *s);

int printf(const char *fmt, ...);
int vprintf(const char *fmt, va_list ap);
int sprintf(char *buf, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list ap);


int getch();
char *gets(char *s);

int vsscanf(const char *buf, const char *fmt, va_list ap);
int sscanf(const char *buf, const char *fmt, ...);
int vscanf(const char *fmt, va_list ap);
int scanf(const char *fmt, ...);



#endif  // _LEANUX_LIBC_STDIO_H

