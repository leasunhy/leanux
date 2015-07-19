#ifndef _LEANUX_SYSCALL_H
#define _LEANUX_SYSCALL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef int (*syscall_t)(int32_t, int32_t, int32_t);

int _syscall();
void syscall();

int sys_execve(int32_t path, int32_t argv, int32_t envp);

int sys_read(int32_t fd, int32_t buffer, int32_t len);
int sys_write(int32_t fd, int32_t buffer, int32_t len);

int sys_tty_putchar_at(int32_t c, int32_t color, int32_t pos);
int sys_tty_putchar_ntimes(int32_t c, int32_t color, int32_t n);
int sys_tty_putchar(int32_t c, int32_t, int32_t);
int sys_print_with_color(int32_t s, int32_t color, int32_t);

int sys_lab_ouch(int32_t _1, int32_t _2, int32_t _3);
int sys_lab_capitalize(int32_t s, int32_t _2, int32_t _3);
int sys_lab_lower(int32_t s, int32_t _2, int32_t _3);
int sys_lab_atoi(int32_t s, int32_t _2, int32_t _3);
int sys_lab_itoa(int32_t num, int32_t buf, int32_t _3);
int sys_lab_puts_at(int32_t s, int32_t row, int32_t column);

#endif

