#ifndef _LEANUX_UTILS_H
#define _LEANUX_UTILS_H

#include <stddef.h>
#include <stdint.h>

void memory_copy(uint8_t *dest, uint8_t *src, size_t count);
void memory_set(void *p, uint8_t value, size_t count);

#endif

