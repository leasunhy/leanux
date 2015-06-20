#ifndef _LEANUX_UTILS_H
#define _LEANUX_UTILS_H

#include <stddef.h>
#include <stdint.h>

void memory_copy(uint8_t *dest, uint8_t *src, size_t count);
void memory_set(void *p, uint8_t value, size_t count);

int string_cmp(const char *sa, const char *sb);

void read_disk(uint32_t sector_no, uint32_t count, void *to);
/* defined in asm_utils.S */
void read_a_sector(uint32_t sector_no, void *to);

#endif

