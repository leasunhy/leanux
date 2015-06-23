#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/leanux.h>
#include <leanux/interrupt.h>
#include <leanux/timer.h>

struct timer_event_t timer_event_list[MAX_TIMER_EVENT];
int32_t timer_event_num;

void timer_init() {
    timer_event_num = 0;
    register_interrupt(0x20, 0x8, (uint32_t)&int_20_timer);
}

void _int_20_timer() {
    send_pic_eoi(0x20);
    /* Since the 0th event will be the scheduler, use reversed order */
    for (int32_t i = timer_event_num - 1; i >= 0; --i) {
        struct timer_event_t *p_event = &timer_event_list[i];
        if (!--p_event->count_down) {
            p_event->count_down = p_event->interval;
            if (p_event->type == TIMER_ONESHOT)
                unregister_timer_event(p_event->event_id);
            (*p_event->handler)();
        }
    }
}

int32_t register_timer_event(void (*handler)(), uint32_t interval,
        enum timer_event_type type) {
    disable_interrupt();
    if (timer_event_num >= MAX_TIMER_EVENT)
        return -1;
    timer_event_list[timer_event_num].event_id = timer_event_num;
    timer_event_list[timer_event_num].count_down = interval;
    timer_event_list[timer_event_num].interval = interval;
    timer_event_list[timer_event_num].handler = handler;
    timer_event_list[timer_event_num].type = type;
    enable_interrupt();
    return timer_event_num++;
}

bool unregister_timer_event(int32_t timer_event_id) {
    disable_interrupt();
    if (timer_event_id < 0 || timer_event_id >= timer_event_num)
        return false;
    --timer_event_num;
    for (int32_t i = timer_event_id; i < timer_event_num; ++i)
        timer_event_list[i] = timer_event_list[i + 1];
    enable_interrupt();
    return true;
}

