#include <stdint.h>

#include <low_level.h>

uint8_t port_byte_in(uint16_t port) {
    uint8_t res;
    __asm__ __volatile__("in al, dx" : "=a"(res) : "d"(port));
    return res;
}

void port_byte_out(uint16_t port, uint8_t data) {
    __asm__ __volatile__("out dx, al" : : "a"(data), "d"(port));
}

uint16_t port_short_in(uint16_t port) {
    uint16_t res;
    __asm__ __volatile__("in ax, dx" : "=a"(res) : "d"(port));
    return res;
}

void port_short_out(uint16_t port, uint16_t data) {
    __asm__ __volatile__("out dx, ax" : : "a"(data), "d"(port));
}


