#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <lib/cirqueue.h>

void cirqueue_init(struct cirqueue *q) {
    q->front = q->end = 0;
}

bool cirqueue_enqueue(struct cirqueue *q, uint16_t entry) {
    size_t count = (q->end - q->front) % _CIRQUEUE_BUF_SIZE;
    if (count == _CIRQUEUE_BUF_SIZE - 1)  /* full */
        return false;
    q->buffer[q->end % _CIRQUEUE_BUF_SIZE] = entry;
    q->end += 1;
    while (q->front >= _CIRQUEUE_BUF_SIZE && q->end >= _CIRQUEUE_BUF_SIZE) {
        q->front -= _CIRQUEUE_BUF_SIZE;
        q->end -= _CIRQUEUE_BUF_SIZE;
    }
    return true;
}

bool cirqueue_front(struct cirqueue *q, uint16_t *entry) {
    if ((q->end - q->front) % _CIRQUEUE_BUF_SIZE == 0)  /* empty */
        return false;
    *entry = q->buffer[q->front % _CIRQUEUE_BUF_SIZE];
    return true;
}

bool cirqueue_serve(struct cirqueue *q, uint16_t *entry) {
    if ((q->end - q->front) % _CIRQUEUE_BUF_SIZE == 0)  /* empty */
        return false;
    *entry = q->buffer[q->front % _CIRQUEUE_BUF_SIZE];
    q->front = (q->front + 1) % _CIRQUEUE_BUF_SIZE;
    return true;
}

