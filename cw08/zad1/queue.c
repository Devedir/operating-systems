#include "queue.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

const static char MAIN_SEM_NAME[] = "/main_sem";
const static char USER_SEM_NAME[] = "/user_sem";
const static char PRINTER_SEM_NAME[] = "/printer_sem";

sem_t* q_main_open() {
    sem_t* sem_addr = sem_open(MAIN_SEM_NAME, O_RDWR);
    if (sem_addr == SEM_FAILED) {
        perror("Semaphore opening error");
        exit(20);
    }
    return sem_addr;
}

sem_t* q_user_open() {
    sem_t* sem_addr = sem_open(USER_SEM_NAME, O_RDWR | O_CREAT, S_IRWXU, QUEUE_CAPACITY);
    if (sem_addr == SEM_FAILED) {
        perror("Semaphore opening error");
        exit(21);
    }
    return sem_addr;
}

sem_t* q_printer_open() {
    sem_t* sem_addr = sem_open(PRINTER_SEM_NAME, O_RDWR | O_CREAT, S_IRWXU, 0);
    if (sem_addr == SEM_FAILED) {
        perror("Semaphore opening error");
        exit(22);
    }
    return sem_addr;
}

sem_t* q_init(queue_t* queue) {
    queue->size = 0;
    queue->before = QUEUE_CAPACITY - 1;
    memset(queue->body, 0, sizeof queue->body);
    sem_t* sem_addr = sem_open(MAIN_SEM_NAME, O_RDWR | O_CREAT, S_IRWXU, 1);
    if (sem_addr == SEM_FAILED) {
        perror("Semaphore opening error");
        exit(20);
    }
    return sem_addr;
}

void q_push(queue_t* queue, char text[TEXT_LEN], sem_t* main_sem, sem_t* user_sem, sem_t* printer_sem){
    sem_wait(user_sem);
    sem_wait(main_sem);

    if (queue->size == QUEUE_CAPACITY)
        return;
    queue->size++;
    int pos = (queue->before + queue->size) % QUEUE_CAPACITY;
    strncpy(queue->body[pos], text, TEXT_LEN);

    sem_post(main_sem);
    sem_post(printer_sem);
}

char* q_pop(queue_t* queue, sem_t* main_sem, sem_t* user_sem, sem_t* printer_sem) {
    sem_wait(printer_sem);
    sem_wait(main_sem);

    if (queue->size == 0)
        return NULL;
    queue->before++;
    queue->before %= QUEUE_CAPACITY;
    queue->size--;
    char* result = queue->body[queue->before];

    sem_post(main_sem);
    sem_post(user_sem);
    return result;
}

void q_sem_close(sem_t* sem) {
    int closing = sem_close(sem);
    if (closing < 0)
        perror("Semaphore closing error");
}

void q_sem_unlink() {
    int unlinking = sem_unlink(MAIN_SEM_NAME)
                  + sem_unlink(USER_SEM_NAME)
                  + sem_unlink(PRINTER_SEM_NAME);
    if (unlinking < 0)
        perror("Semaphore unlinking error");
}
