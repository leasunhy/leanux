#ifndef _LEANUX_LEANUX_H
#define _LEANUX_LEANUX_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct GLOBAL_DESC {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t type            : 4;
    uint8_t s               : 1;
    uint8_t dpl             : 2;
    uint8_t present         : 1;
    uint8_t limit_high      : 4;
    uint8_t avail_to_sys    : 1;
    uint8_t zero            : 1;
    uint8_t d_or_b          : 1;
    uint8_t granularity     : 1;
    uint8_t base_high;
}__attribute__((packed));

extern struct GLOBAL_DESC *GDT;

void enter_shell();

#endif

