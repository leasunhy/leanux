#ifndef _LEANUX_TIMER_H
#define _LEANUX_TIMER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/leanux.h>

enum timer_event_type {TIMER_ONESHOT = 1, TIMER_CONSTANT};

struct timer_event_t {
    int32_t event_id;
    uint32_t count_down;
    uint32_t interval;
    enum timer_event_type type;
    void (*handler)();
};

void timer_init();
void int_20_timer();
void int_20_timer_end();
int32_t register_timer_event(void (*handler)(), uint32_t interval,
        enum timer_event_type type);
bool unregister_timer_event(int32_t timer_event_id);

#endif

