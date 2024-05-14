#define _POSIX_C_SOURCE 200112L // for ftruncate() to work

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "queue.h"

void parse_input(int argc, char* argv[], int* no_users, int* no_printers) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments required: number of users and printers.\n");
        exit(1);
    }

    errno = 0;
    char* endptr = argv[1];
    *no_users = (int) strtol(argv[1], &endptr, 10);
    if (argv[1] == endptr) {
        fprintf(stderr, "The first argument is not a valid number.\n");
        exit(2);
    }
    *no_printers = (int) strtol(argv[2], &endptr, 10);
    if (argv[2] == endptr) {
        fprintf(stderr, "The second argument is not a valid number.\n");
        exit(2);
    }
    if (errno != 0) {
        perror("Arguments parsing error");
        exit(3);
    }
    if (*no_users <= 0 || *no_printers <= 0) {
        fprintf(stderr, "Numbers must be positive.\n");
        exit(4);
    }
}

int main(int argc, char* argv[]) {
    int no_users, no_printers;
    parse_input(argc, argv, &no_users, &no_printers);

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
