#include <stddef.h>
#include <stdint.h>

#include <lib/utils.h>

void memory_copy(uint8_t *dest, uint8_t *src, size_t count) {
    for (size_t i = 0; i != count; ++i)
        *dest++ = *src++;
}

void memory_set(void *addr, uint8_t value, size_t count) {
    uint8_t *dest = (uint8_t*)addr;
    for (size_t i = 0; i < count; ++i)
        *dest++ = value;
}

