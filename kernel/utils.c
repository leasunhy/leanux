#include <stddef.h>
#include <stdint.h>

#include <utils.h>

void memory_copy(uint8_t *dest, uint8_t *src, size_t count) {
    for (size_t i = 0; i != count; ++i)
        *dest++ = *src++;
}

