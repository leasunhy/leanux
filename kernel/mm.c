#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/interrupt.h>
#include <leanux/leanux.h>
#include <lib/utils.h>
#include <lib/printk.h>

#include <leanux/mm.h>

PDE *kernel_pd;
PTE *kernel_pt;

uint32_t num_of_page;
uint32_t kernel_break;
uint32_t kernel_page_break;
uint8_t *mem_map;
size_t memmap_size;

static inline uint32_t pt_addr(void *addr) {
    return (uint32_t)addr >> 12;
}

inline void flush_tlb() {
    __asm__ __volatile__(
            "mov eax, cr3;"
            "mov cr3, eax;"
            ::: "eax"
    );
}

void page_init() {
    kernel_pd = &page_directory;
    kernel_pt = &kernel_page_table;

    memory_set(kernel_pd, 0, 4096);

    kernel_pd[0] = make_pde(pt_addr(kernel_pt), false, true);

    for (PAGE *i = 0; i <= &KERNEL_END; ++i)
        kernel_pt[pt_addr(i)] = make_pte(pt_addr(i), true, false, true);

    /* add handler for page faults */
    register_interrupt(0x0e, 0x8, (uint32_t)&int_page_fault);

    /* load cr3 and turn paging on! */
    /* cr0.wp = 1, so supervisor code can't write to read-only mem */
    __asm__(
        "mov cr3, %0;"
        "mov eax, cr0;"
        "or eax, 0x80010000;"
        "mov cr0, eax;"
        :
        : "r"(kernel_pd)
        : "eax"
    );
}

void mm_init(uint32_t max_kb_installed) {
    num_of_page = max_kb_installed >> 2;  /* kb = 2^10 b */
    memmap_size = num_of_page + 1;
    kernel_break = ((((uint32_t)&KERNEL_END) >> 12) + 1) << 12;
    mem_map = (uint8_t*)kernel_break;
    kernel_page_break = ((kernel_break + memmap_size) >> 12) + 1;

    for (uint32_t i = (kernel_break >> 12) - 1; i <= kernel_page_break; ++i)
        /*mm_mmap(kernel_pd, i, i, true, false, true);*/
        kernel_pt[i] = make_pte(i, true, false, true);
    kernel_break += memmap_size;
    for (uint32_t i = 0; i <= kernel_page_break + 1; ++i)
        mem_map[i] = 1;
    for (uint32_t i = kernel_page_break + 2; i < num_of_page; ++i)
        mem_map[i] = 0;
    flush_tlb();
}

void mm_mmap(PDE *pd, uint32_t vir_page, uint32_t phy_page,
        bool global, bool user, bool read_write) {
    uint32_t pde_no = vir_page >> 10;
    uint32_t pte_no = vir_page & 0x3ff;
    PDE *pde = &pd[pde_no];
    /* check if page directory entry present */
    if (!(*pde & 1)) {
        /* if not, allocate one */
        /* kernel is limited to the 1023th page */
        uint32_t new_pt_page = alloc_page(0, 1023);
        /* inform kernel */
        mm_mmap(kernel_pd, new_pt_page, new_pt_page, true, false, true);
        memory_set((void*)(new_pt_page << 12), 0, 4096);
        *pde = make_pde(new_pt_page, user, read_write);
    }
    PTE *pt = (PTE*)(*pde & 0xFFFFF000);
    pt[pte_no] = make_pte(phy_page, global, user, read_write);
    ++mem_map[phy_page];
    flush_tlb();
}


PDE make_pde(uint32_t pt_page_no, bool user, bool read_write) {
    PDE res = 0;
    res |= pt_page_no << 12;
    res |= user << 2;
    res |= read_write << 1;
    res |= 1;  /* always present */
    return res;
}

PTE make_pte(uint32_t pt_page_no, bool global, bool user, bool read_write) {
    PDE res = 0;
    res |= global << 8;
    res |= pt_page_no << 12;
    res |= user << 2;
    res |= read_write << 1;
    res |= 1;  /* always present */
    return res;
}

uint32_t alloc_page(uint32_t lower_bound, uint32_t upper_bound) {
    uint32_t i = lower_bound;
    while (i <= upper_bound && mem_map[i] != 0)
        ++i;
    return i;
}

void _int_page_fault(uint32_t addr, uint32_t errcode) {
    printk("addr = %d, errcode = %d", addr, errcode);
    __asm__ __volatile__("cli; hlt;");
}

