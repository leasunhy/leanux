#ifndef _LEANUX_CIRQUEUE_H
#define _LEANUX_CIRQUEUE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define _CIRQUEUE_BUF_SIZE 64

struct cirqueue {
    uint16_t buffer[_CIRQUEUE_BUF_SIZE];
    size_t front, end;
};

void cirqueue_init(struct cirqueue *q);

bool cirqueue_enqueue(struct cirqueue *q, uint16_t entry);
bool cirqueue_front(struct cirqueue *q, uint16_t *entry);
bool cirqueue_serve(struct cirqueue *q, uint16_t *entry);

#endif

