#define _POSIX_C_SOURCE 200112L // for ftruncate() to work

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

    setbuf(stdout, NULL);
    sem_t* sem = q_open();
    char* str = q_pop(queue, sem);
    if (str == NULL) {
        fprintf(stderr, "Empty queue!\n");
    } else {
        for (int i = 0; i < 10; i++) {
            printf("%c", str[i]);
            sleep(1);
        }
        printf("\n");
    }
    q_sem_close(sem);

    int unmapping = munmap(queue, sizeof(queue_t));
    if (unmapping == -1) {
        perror("Memory unmapping failed");
        shm_unlink(shm_name);
        return 13;
    }

    return 0;
}
