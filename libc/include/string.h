#ifndef _LEANUX_LIBC_STRING_H
#define _LEANUX_LIBC_STRING_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
char *strncpy(char *dest, const char *src, size_t n);
char *strcpy(char *dest, const char *src);


#endif  // _LEANUX_LIBC_STRING_H

