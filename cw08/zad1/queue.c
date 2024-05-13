#include "queue.h"

#include <string.h>

queue_t q_create() {
    queue_t new_queue = {
            .size = 0,
            .before = QUEUE_CAPACITY - 1,
            .body = {{0}}
    };
    return new_queue;
}

void q_init(queue_t* queue) {
    queue->size = 0;
    queue->before = QUEUE_CAPACITY - 1;
    memset(queue->body, 0, sizeof queue->body);
}

void q_push(queue_t* queue, char text[TEXT_LEN]) {
    if (queue->size == QUEUE_CAPACITY)
        return;
    queue->size++;
    int pos = (queue->before + queue->size) % QUEUE_CAPACITY;
    strncpy(queue->body[pos], text, TEXT_LEN);
}

char* q_pop(queue_t* queue) {
    if (queue->size == 0)
        return NULL;
    queue->before++;
    queue->before %= QUEUE_CAPACITY;
    queue->size--;
    return queue->body[queue->before];
}
