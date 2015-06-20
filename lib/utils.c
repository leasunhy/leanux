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

int string_cmp(const char *sa, const char *sb) {
    while (*sa && *sb && *sa == *sb)
        ++sa, ++sb;
    if (*sa < *sb)
        return -1;
    else if (*sa > *sb)
        return 1;
    else
        return 0;
}

void read_disk(uint32_t sector_no, uint32_t count, void *to) {
    uint32_t i = 0;
    while (i++ != count) {
        read_a_sector(sector_no, to);
        sector_no += 1;
        to = (void *)((uint32_t)to + 512);
    }
}

