#include "queue.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

const static char SEM_NAME[] = "/main_sem";

sem_t* q_open() {
    sem_t* sem_addr = sem_open(SEM_NAME, O_RDWR);
    if (sem_addr == SEM_FAILED) {
        perror("Semaphore opening error");
        exit(20);
    }
    return sem_addr;
}

sem_t* q_init(queue_t* queue) {
    queue->size = 0;
    queue->before = QUEUE_CAPACITY - 1;
    memset(queue->body, 0, sizeof queue->body);
    sem_t* sem_addr = sem_open(SEM_NAME, O_RDWR | O_CREAT, S_IRWXU, 1);
    if (sem_addr == SEM_FAILED) {
        perror("Semaphore opening error");
        exit(20);
    }
    return sem_addr;
}

void q_push(queue_t* queue, char text[TEXT_LEN], sem_t* sem_addr) {
    sem_wait(sem_addr);
    if (queue->size == QUEUE_CAPACITY)
        return;
    queue->size++;
    int pos = (queue->before + queue->size) % QUEUE_CAPACITY;
    strncpy(queue->body[pos], text, TEXT_LEN);
    sem_post(sem_addr);
}

char* q_pop(queue_t* queue, sem_t* sem_addr) {
    sem_wait(sem_addr);
    if (queue->size == 0)
        return NULL;
    queue->before++;
    queue->before %= QUEUE_CAPACITY;
    queue->size--;
    char* result = queue->body[queue->before];
    sem_post(sem_addr);
    return result;
}

void q_sem_close(sem_t* sem_addr) {
    int closing = sem_close(sem_addr);
    if (closing == -1)
        perror("Semaphore closing error");
}

void q_sem_unlink() {
    int unlinking = sem_unlink(SEM_NAME);
    if (unlinking == -1)
        perror("Semaphore unlinking error");
}
