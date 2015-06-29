#include <stdint.h>

#include <leanux/leanux.h>
#include <leanux/interrupt.h>
#include <leanux/syscall.h>
#include <leanux/low_level.h>
#include <leanux/mm.h>
#include <leanux/timer.h>
#include <leanux/sched.h>
#include <drivers/tty.h>
#include <drivers/keyboard.h>
#include <lib/printk.h>
#include <lib/utils.h>

void animation();
void load_program(struct process_t *proc, uint8_t prog_no);
int exec_program(uint8_t prog_no);
void start_shell();

void kernel_main(void *multiboot_info) {
    disable_interrupt();
    tty_init();
    idt_init();
    pic_init();
    page_init();
    mm_init(((uint32_t*)multiboot_info)[2]);

    keyboard_init();
    timer_init();
    sched_init();

    register_interrupt(0x80, 0x8, (uint32_t)&syscall);

    register_timer_event(animation, 7, TIMER_CONSTANT);

    start_shell();

    enable_interrupt();

    while (1) {
        ;
    }
}

/* the shell is the 10th program on the disk image */
void start_shell() {
    exec_program(10);
}

void load_program(struct process_t *proc, uint8_t prog_no) {
    /* the physical page # is also the linear page # in kernel_pd */
    uint32_t phy_page_no = proc->page_no;
    void *prog_page = (void *)(phy_page_no << 12);
    read_disk(2 + 8 * (prog_no - 1), 8, prog_page);
}

int exec_program(uint8_t prog_no) {
    uint32_t proc_struct_page_no = alloc_page(1024, num_of_page);
    mm_mmap(kernel_pd, proc_struct_page_no, proc_struct_page_no, true, false, true);
    struct process_t *proc = (struct process_t *)(proc_struct_page_no << 12);
    init_process(proc);
    mm_mmap(proc->page_dir, proc_struct_page_no, proc_struct_page_no, true, false, true);
    load_program(proc, prog_no);
    add_process(proc);
    return 0;
}

const char *anim_seq = "\\|/-";
const int anim_seq_len = 4;
int anim_count = 0;

void animation() {
    anim_count += 1;
    anim_count %= anim_seq_len;
    tty_put_entry_at(anim_seq[anim_count], 0x0F, 79, 24);
}


