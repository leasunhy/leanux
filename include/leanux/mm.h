#ifndef _LEANUX_MM_H
#define _LEANUX_MM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/leanux.h>

typedef uint8_t PAGE[4096];
typedef uint32_t PDE;
typedef uint32_t PTE;

void page_init();
void mm_init(uint32_t max_kb_installed);

void int_page_fault();

PDE make_pde(uint32_t pt_page_no, bool user, bool read_write);
PTE make_pte(uint32_t pt_page_no, bool global, bool user, bool read_write);

static inline void switch_pd(PDE *pd_to) {
    __asm__ __volatile__(
        "mov cr3, %0;"
        "mov eax, cr0;"
        :
        : "r"(pd_to)
        : "eax"
    );
}

static inline PDE *current_pd() {
    PDE *res;
    __asm__ __volatile__(
        "mov %0, cr3"
        : "=r"(res)
    );
    return res;
}

void mm_mmap(PDE *pd, uint32_t vir_page, uint32_t phy_page,
        bool global, bool user, bool read_write);

PDE *copy_pd_and_pts(const PDE *pd_from);

uint32_t alloc_page(uint32_t lower_bound, uint32_t upper_bound);

extern PAGE KERNEL_RO_START;
extern PAGE KERNEL_RW_START;
extern PAGE KERNEL_RO_END;
extern PAGE KERNEL_RW_END;
extern PAGE KERNEL_END;

extern PDE *kernel_pd;
extern PTE *kernel_pt;

extern size_t num_of_page;
extern uint8_t *mem_map;
extern size_t memmap_size;
extern uint32_t kernel_break;
extern uint32_t kernel_page_break;

/* defined in asm */
extern PDE page_directory;
extern PTE kernel_page_table;

#endif

