#ifndef _LEANUX_LOW_LEVEL_H
#define _LEANUX_LOW_LEVEL_H

#include <stdint.h>

uint8_t port_byte_in(uint16_t port);
void port_byte_out(uint16_t port, uint8_t data);

uint16_t port_short_in(uint16_t port);
void port_short_out(uint16_t port, uint16_t data);

#endif

