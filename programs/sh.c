#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <drivers/vga.h>
#include <lib/utils.h>

#include <leanux/sys.h>

const char *shell_prompt = "leanux> ";
const size_t shell_cmd_maxlen = 72;  /* VGA_WIDTH - len(prompt) */

void shell_print_header();
void shell_print_error();
void shell_print_info();

void shell_process_cmd(const char *s, size_t len);


/* entry for shell. if returned, will again be called by the kernel */
int main() {
    shell_print_header();
    char buf[shell_cmd_maxlen + 10];
    while (true) {
        print_with_color(shell_prompt, COLOR_WHITE);
        size_t len = read(0, buf, shell_cmd_maxlen);
        shell_process_cmd(buf, len);
    }
    return 0;
}

/* process the command line */
void shell_process_cmd(const char *s, size_t len) {
    if (len == shell_cmd_maxlen) {
        shell_print_error("Command too long. Discarded.");
    }
    if (fork() == 0) {
        if (execve(s, NULL, NULL) == 0) {
            wait();
            _exit(0);
        } else {
            shell_print_error("No such command.");
            _exit(0);
        }
    }
    wait();
}


/* below are auxilary functions, just for convience :-) */

void shell_print_boxed_line(const char *s, uint8_t color) {
    s_putchar_ntimes('|', COLOR_LIGHT_BLUE, 1);
    s_putchar_ntimes(' ', 0, 5);
    int count = 0;
    while (*s) {
        s_putchar_ntimes(*s, color, 1);
        ++s;
        ++count;
    }
    s_putchar_ntimes(' ', 0, VGA_WIDTH - count - 5 - 1 - 1);
    s_putchar_ntimes('|', COLOR_LIGHT_BLUE, 1);
}

void shell_print_header() {
    s_putchar_ntimes('-', COLOR_LIGHT_BLUE, VGA_WIDTH);
    shell_print_boxed_line("", 0);
    shell_print_boxed_line("Welcome to LeaNux!", COLOR_LIGHT_GREEN);
    shell_print_boxed_line("    by Liu Siyuan, 13349073", COLOR_LIGHT_RED);
    shell_print_boxed_line("        Have a nice day~! :D", COLOR_LIGHT_BROWN);
    shell_print_boxed_line("", 0);
    s_putchar_ntimes('-', 0x09, VGA_WIDTH);
    s_putchar('\n');
    s_putchar('\n');
}

void shell_print_error(const char *s) {
    print_with_color("ERROR: ", COLOR_LIGHT_RED);
    print_with_color(s, COLOR_LIGHT_RED);
    s_putchar('\n');
}

void shell_print_info(const char *s) {
    print_with_color("INFO: ", COLOR_WHITE);
    print_with_color(s, COLOR_WHITE);
    s_putchar('\n');
}

