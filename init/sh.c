#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <leanux/mm.h>
#include <leanux/timer.h>
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
    register_timer_event(shell_animation, 3, TIMER_CONSTANT);
    shell_print_header();
    char buf[shell_cmd_maxlen + 10];
    while (true) {
        tty_writestring_colored(shell_prompt, COLOR_WHITE);
        size_t len = tty_read(buf, shell_cmd_maxlen);
        shell_process_cmd(buf, len);
    }
}

PDE *load_program(uint8_t prog_no) {
    if (prog_no != 1)
        return NULL;
    PDE *user_pd = copy_pd_and_pts(kernel_pd);
    /* the physical page # is also the linear page # in kernel_pd */
    uint32_t phy_page_no = alloc_page(1024, num_of_page);
    mm_mmap(user_pd, USER_PROG_ADDR >> 12, phy_page_no,
            true, false, true);
    mm_mmap(kernel_pd, phy_page_no, phy_page_no, true, false, true);
    void *prog_page = (void *)(phy_page_no << 12);
    read_disk(2 + 8 * (prog_no - 1), 8, prog_page);
    return user_pd;
}

int exec_program(uint8_t prog_no) {
    PDE *user_pd = load_program(prog_no);
    if (!user_pd)
        return -1;
    switch_pd(user_pd);
    call(USER_PROG_ADDR);
    switch_pd(kernel_pd);
    return 0;
}

/* process the command line */
void shell_process_cmd(const char *s, size_t len) {
    if (len == shell_cmd_maxlen) {
        shell_print_error("Command too long. Discarded.");
    }
    if (string_cmp(s, "prog1") == 0) {
        exec_program(1);
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

