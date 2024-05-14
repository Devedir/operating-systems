#define _POSIX_C_SOURCE 200112L // for ftruncate() to work

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "queue.h"

int main(void) {
    const char shm_name[] = "/printing_queue";
    int shmd = shm_open(shm_name, O_RDWR | O_CREAT, S_IRWXU);
    if (shmd == -1) {
        perror("Shared memory creating error");
        return 10;
    }

    int truncating = ftruncate(shmd, sizeof(queue_t));
    if (truncating == -1) {
        perror("Error truncating shared memory.");
        shm_unlink(shm_name);
        return 11;
    }

    queue_t* queue = mmap(NULL, sizeof(queue_t), PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);
    if (queue == MAP_FAILED) {
        perror("Memory mapping failed");
        shm_unlink(shm_name);
        return 12;
    }

    sem_t* sem = q_init(queue);
    getchar(); // waiting to manually end
    q_sem_close(sem);
    q_sem_unlink();

    int unmapping = munmap(queue, sizeof(queue_t));
    if (unmapping == -1) {
        perror("Memory unmapping failed");
        shm_unlink(shm_name);
        return 13;
    }

    int unlinking = shm_unlink(shm_name);
    if (unlinking == -1) {
        perror("Shared memory unlinking error");
        return 14;
    }

    return 0;
}
