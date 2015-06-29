#ifndef _LEANUX_UTILS_H
#define _LEANUX_UTILS_H

#include <stddef.h>
#include <stdint.h>

void memory_copy(void *dest, void *src, size_t count);
void memory_set(void *p, uint8_t value, size_t count);

int string_cmp(const char *sa, const char *sb);
int string_ncmp(const char *sa, const char *sb, size_t n);

void read_disk(uint32_t sector_no, uint32_t count, void *to);

/* defined in asm_utils.S */
void read_a_sector(uint32_t sector_no, void *to);

size_t kitoa(char *buf, int i, int base);
int katoi(const char *ns);

inline void call(uint32_t addr) {
    __asm__ __volatile__("call %0" ::"m"(addr));
}

inline void jmp(uint32_t addr) {
    __asm__ __volatile__("jmp %0" :: "m"(addr));
}

#endif

