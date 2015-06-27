#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <leanux/mm.h>
#include <leanux/timer.h>
#include <leanux/sched.h>
#include <leanux/interrupt.h>
#include <drivers/vga.h>
#include <drivers/tty.h>
#include <lib/printk.h>
#include <lib/utils.h>

#define printf printk

const char *shell_prompt = "leanux> ";
const size_t shell_cmd_maxlen = 72;  /* VGA_WIDTH - len(prompt) */

const char *anim_seq = "\\|/-";
const int anim_seq_len = 4;
int anim_count = 0;

void shell_print_header();
void shell_print_error();
void shell_print_info();

void shell_process_cmd(const char *s, size_t len);
void shell_animation();


/* entry for shell. if returned, will again be called by the kernel */
void shell_main() {
    register_timer_event(shell_animation, 7, TIMER_CONSTANT);
    shell_print_header();
    char buf[shell_cmd_maxlen + 10];
    while (true) {
        tty_writestring_colored(shell_prompt, COLOR_WHITE);
        size_t len = tty_read(buf, shell_cmd_maxlen);
        shell_process_cmd(buf, len);
    }
}

void load_program(struct process_t *proc, uint8_t prog_no) {
    /* the physical page # is also the linear page # in kernel_pd */
    uint32_t phy_page_no = proc->page_no;
    void *prog_page = (void *)(phy_page_no << 12);
    read_disk(2 + 8 * (prog_no - 1), 8, prog_page);
}

int exec_program(uint8_t prog_no) {
    if (prog_no != 1 && prog_no != 9)
        return -1;
    uint32_t proc_page_no = alloc_page(1024, num_of_page);
    mm_mmap(kernel_pd, proc_page_no, proc_page_no, true, false, true);
    struct process_t *proc = (struct process_t *)(proc_page_no << 12);
    init_process(proc);
    mm_mmap(proc->page_dir, proc_page_no, proc_page_no, true, false, true);
    load_program(proc, prog_no);
    add_process(proc);
    return 0;
}

/* process the command line */
void shell_process_cmd(const char *s, size_t len) {
    if (len == shell_cmd_maxlen) {
        shell_print_error("Command too long. Discarded.");
    }
    if (string_cmp(s, "prog1") == 0) {
        disable_interrupt();
        exec_program(1);
        enable_interrupt();
        return;
    }
    shell_print_error("No such command.");
}


/* below are auxilary functions, just for convience :-) */

void shell_print_boxed_line(const char *s, uint8_t color) {
    tty_putchar_ntimes('|', COLOR_LIGHT_BLUE, 1);
    tty_putchar_ntimes(' ', 0, 5);
    int count = 0;
    while (*s) {
        tty_putchar_ntimes(*s, color, 1);
        ++s;
        ++count;
    }
    tty_putchar_ntimes(' ', 0, VGA_WIDTH - count - 5 - 1 - 1);
    tty_putchar_ntimes('|', COLOR_LIGHT_BLUE, 1);
}

void shell_print_header() {
    tty_putchar_ntimes('-', COLOR_LIGHT_BLUE, VGA_WIDTH);
    shell_print_boxed_line("", 0);
    shell_print_boxed_line("Welcome to LeaNux!", COLOR_LIGHT_GREEN);
    shell_print_boxed_line("    by Liu Siyuan, 13349073", COLOR_LIGHT_RED);
    shell_print_boxed_line("        Have a nice day~! :D", COLOR_LIGHT_BROWN);
    shell_print_boxed_line("", 0);
    tty_putchar_ntimes('-', 0x09, VGA_WIDTH);
    tty_writestring_colored("\n", COLOR_LIGHT_RED);
    tty_writestring_colored("\n", COLOR_LIGHT_RED);
}

void shell_print_error(const char *s) {
    tty_writestring_colored("ERROR: ", COLOR_LIGHT_RED);
    tty_writestring_colored(s, COLOR_LIGHT_RED);
    tty_putchar('\n');
}

void shell_print_info(const char *s) {
    tty_writestring_colored("INFO: ", COLOR_WHITE);
    tty_writestring_colored(s, COLOR_WHITE);
    tty_putchar('\n');
}

void shell_animation() {
    anim_count += 1;
    anim_count %= anim_seq_len;
    tty_put_entry_at(anim_seq[anim_count], 0x0F, 79, 24);
}

#undef printf

