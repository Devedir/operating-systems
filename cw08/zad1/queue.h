#include <semaphore.h>

#define TEXT_LEN 11 // including '\0'
#define QUEUE_CAPACITY 5

typedef struct queue_t {
    int size;
    int before;
    char body[QUEUE_CAPACITY][TEXT_LEN];
} queue_t;

sem_t* q_open(); // Use in all processes except for the main one

sem_t* q_init(queue_t* queue); // Use in the main process

void q_push(queue_t* queue, char text[TEXT_LEN], sem_t* sem_addr);

char* q_pop(queue_t* queue, sem_t* sem_addr);

void q_sem_close(sem_t* sem_addr); // Use in all processes

void q_sem_unlink(); // Use in the main process
