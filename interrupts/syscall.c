#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/interrupt.h>
#include <drivers/tty.h>

#include <leanux/syscall.h>

const syscall_t syscall_table[] = {
    NULL,
    NULL,
    NULL,
    sys_read,
    sys_write,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,  /* 10 */
    NULL,
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
    NULL,
    NULL,
    NULL,  /* 30 */
    sys_print_with_color
};

int _syscall() {
    register int syscall_no asm("eax");
    int32_t _1, _2, _3;
    __asm__ __volatile__(
            "mov %1, ebx;"
            "mov %2, ecx;"
            "mov %3, edx;"
            : "=m"(_1), "=m"(_2), "=m"(_3)
            : "m"(_1), "m"(_2), "m"(_3)
            :
    );
    return syscall_table[syscall_no](_1, _2, _3);
}

int sys_read(int32_t fd, int32_t buffer, int32_t len) {
    if (fd == 0)  /* stdin */
        return tty_read((char *)buffer, (size_t)len);
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

