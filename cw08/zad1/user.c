#define _POSIX_C_SOURCE 200112L // for ftruncate() to work

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "queue.h"

int main(void) {
    const char shm_name[] = "/printing_queue";
    int shmd = shm_open(shm_name, O_RDWR, S_IRWXU);
    if (shmd == -1) {
        perror("Shared memory creating error");
        return 10;
    }

    int truncating = ftruncate(shmd, sizeof(queue_t));
    if (truncating == -1) {
        perror("Error truncating shared memory.");
        return 11;
    }

    queue_t* queue = mmap(NULL, sizeof(queue_t), PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);
    if (queue == MAP_FAILED) {
        perror("Memory mapping failed");
        return 12;
    }

    sem_t* main_sem = q_main_open();
    sem_t* user_sem = q_user_open();
    sem_t* printer_sem = q_printer_open();

    srand(time(NULL));
    while (1) {
        char text[TEXT_LEN] = {0};
        for (int i = 0; i < 10; i++) {
            text[i] = 'a' + rand() % 26;
        }
        q_push(queue, text, main_sem, user_sem, printer_sem);
        sleep(rand() % 60);
    }

    q_sem_close(main_sem);
    q_sem_close(user_sem);
    q_sem_close(printer_sem);

    int unmapping = munmap(queue, sizeof(queue_t));
    if (unmapping == -1) {
        perror("Memory unmapping failed");
        shm_unlink(shm_name);
        return 13;
    }

    return 0;
}
