#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <drivers/vga.h>
#include <drivers/tty.h>
#include <lib/printk.h>

#define printf printk

const char *shell_prompt = "leanux> ";
const size_t shell_cmd_maxlen = 72;  /* VGA_WIDTH - len(prompt) */

void shell_print_header();
void shell_print_error();
void shell_print_info();

void shell_process_cmd(const char *s, size_t len);


/* entry for shell. if returned, will again be called by the kernel */
void shell_main() {
    shell_print_header();
    char buf[shell_cmd_maxlen + 10];
    while (true) {
        tty_writestring_colored(shell_prompt, COLOR_WHITE);
        size_t len = tty_read(buf, shell_cmd_maxlen);
        shell_process_cmd(buf, len);
    }
}

/* process the command line */
void shell_process_cmd(const char *s, size_t len) {
    if (len == shell_cmd_maxlen) {
        shell_print_error("Command too long. Discarded.");
    }
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

#undef printf

