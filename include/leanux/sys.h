#ifndef _LEANUX_SYS_H
#define _LEANUX_SYS_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static inline int read(unsigned int fd, char *buffer, size_t len) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res), "=m"(*buffer)
            : "a"(3), "b"(fd), "c"(buffer), "d"(len)
    );
    return res;
}

static inline int write(unsigned int fd, const char *buffer, size_t len) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(4), "b"(fd), "c"(buffer), "d"(len)
    );
    return res;
}

static inline int execve(const char *path, char *const argv[], char *const envp[]) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(11), "b"(path), "c"(argv), "d"(envp)
    );
    return res;
}

static inline int fork() {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(2)
    );
    return res;
}

static inline int shared_fork() {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(28)
    );
    return res;
}

static inline int wait() {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(7), "b"(-1), "c"(NULL), "d"(0)
    );
    return res;
}

static inline int s_putchar(const char c) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(30), "b"(c)
    );
    return res;
}

static inline int s_putchar_ntimes(const char c, uint8_t color, size_t n) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(29), "b"(c), "c"(color), "d"(n)
    );
    return res;
}

static inline int print_with_color(const char *buffer, uint8_t color) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(31), "b"(buffer), "c"(color)
    );
    return res;
}

static inline int lab_ouch() {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(32)
    );
    return res;
}

static inline int lab_capitalize(char *s) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res), "=m"(*s)
            : "a"(33), "b"(s)
    );
    return res;
}

static inline int lab_lower(char *s) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res), "=m"(*s)
            : "a"(34), "b"(s)
    );
    return res;
}

static inline int lab_atoi(const char *s) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(35), "b"(s)
    );
    return res;
}

static inline int lab_itoa(int num, char *buf) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res), "=m"(*buf)
            : "a"(36), "b"(num), "c"(buf)
    );
    return res;
}

static inline int lab_puts_at(const char *s, uint32_t row, uint32_t column) {
    int res;
    __asm__ __volatile__(
            "int 0x80;"
            : "=a"(res)
            : "a"(37), "b"(s), "c"(row), "d"(column)
    );
    return res;
}

static inline void _exit(int32_t code) {
    __asm__ __volatile__(
            "int 0x80;"
            :
            : "a"(1), "b"(code)
    );
}

#endif

