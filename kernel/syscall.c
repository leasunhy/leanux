#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/interrupt.h>
#include <lib/utils.h>
#include <drivers/tty.h>

#include <leanux/syscall.h>
#include <leanux/sched.h>
#include <leanux/mm.h>
#include <lib/utils.h>

const syscall_t syscall_table[] = {
    NULL,
    sys_exit,
    sys_fork,
    sys_read,
    sys_write,
    NULL,
    NULL,
    sys_waitpid,
    NULL,
    NULL,
    NULL,  /* 10 */
    sys_execve,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,  /* 20 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    sys_shared_fork,
    sys_tty_putchar_ntimes,
    sys_tty_putchar,  /* 30 */
    sys_print_with_color,
    sys_lab_ouch,     /* 32: below is lab-5 related */
    sys_lab_capitalize,
    sys_lab_lower,
    sys_lab_atoi,
    sys_lab_itoa,
    sys_lab_puts_at,
};

int _syscall() {
    register int syscall_no asm("eax");
    int32_t _1, _2, _3;
    __asm__ __volatile__(
            "mov %0, ebx;"
            "mov %1, ecx;"
            "mov %2, edx;"
            : "=m"(_1), "=m"(_2), "=m"(_3)
    );
    return syscall_table[syscall_no](_1, _2, _3);
}

int sys_execve(int32_t ipath, int32_t iargv, int32_t ienvp) {
    const char *path = (const char *)ipath;
    char *const *argv = (char *const *)iargv;
    char *const *envp = (char *const *)ienvp;
    int prog_no = 0;
    if (string_ncmp(path, "prog", 4) == 0)
        prog_no = katoi(path + 4);
    else
        return -1;
    if (!(prog_no >= 1 && prog_no <= 9))
        return -1;
    read_disk(2 + 8 * (prog_no - 1), 8, (void *)USER_PROG_ADDR);
    /*add_process(proc);*/
    __asm__ __volatile__("jmp %0": : "i"(USER_PROG_ADDR));
    return 0;
}

int sys_read(int32_t fd, int32_t buffer, int32_t len) {
    if (fd == 0)  /* stdin */
        return tty_read((char *)buffer, (size_t)len);
    return 0;
}

int sys_tty_putchar(int32_t c, int32_t _2, int32_t _3) {
    tty_putchar((char)c);
    return 0;
}

int sys_tty_putchar_ntimes(int32_t c, int32_t color, int32_t n) {
    tty_putchar_ntimes((char)c, (uint8_t)color, (size_t)n);
    return 0;
}

int sys_write(int32_t fd, int32_t buffer, int32_t len) {
    if (fd == 1 || fd == 2)  /* stdout */
        tty_write((const char *)buffer, (size_t)len);
    return 0;
}

int sys_print_with_color(int32_t s, int32_t color, int32_t _3) {
    tty_writestring_colored((const char *)s, (uint8_t)color);
    return 0;
}

int sys_lab_ouch(int32_t _1, int32_t _2, int32_t _3) {
    tty_writestring_at("ouch", 0x0f, 12, 60);
    return 0;
}

int sys_lab_capitalize(int32_t s, int32_t _2, int32_t _3) {
    char *str = (char *)s;
    while (*str) {
        if (*str <= 'z' && *str >= 'a')
            *str = *str - 'a' + 'A';
        str++;
    }
    return 0;
}

int sys_lab_lower(int32_t s, int32_t _2, int32_t _3) {
    char *str = (char *)s;
    while (*str) {
        if (*str <= 'Z' && *str >= 'A')
            *str = *str - 'A' + 'a';
        str++;
    }
    return 0;
}

int sys_lab_atoi(int32_t s, int32_t _2, int32_t _3) {
    const char *ns = (const char *)s;
    int res = katoi(ns);
    return res;
}

int sys_lab_itoa(int32_t num, int32_t buf, int32_t _3) {
    char *s = (char *)buf;
    kitoa(s, num, 10);
    return 0;
}

int sys_lab_puts_at(int32_t s, int32_t row, int32_t column) {
    tty_writestring_at((const char*)s, 0x0f, row, column);
    return 0;
}

