#include <semaphore.h>

#define TEXT_LEN 11 // including '\0'
#define QUEUE_CAPACITY 5

typedef struct queue_t {
    int size;
    int before;
    char body[QUEUE_CAPACITY][TEXT_LEN];
} queue_t;

// Use in all processes except for the main one:
sem_t* q_main_open();
sem_t* q_user_open();
sem_t* q_printer_open();

sem_t* q_init(queue_t* queue); // Use in the main process

void q_push(queue_t* queue, char text[TEXT_LEN], sem_t* main_sem, sem_t* user_sem, sem_t* printer_sem);

char* q_pop(queue_t* queue, sem_t* main_sem, sem_t* user_sem, sem_t* printer_sem);

void q_sem_close(sem_t* main_sem); // Use in all processes

void q_sem_unlink(); // Use in the main process
