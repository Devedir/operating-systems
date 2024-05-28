#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NO_REINDEERS 9
#define NO_YEARS 3

int counter = 0;
pthread_mutex_t counterMutex;
pthread_cond_t counterCond;

void* santa(void* arg) {
    for (int i = 0; i < NO_YEARS; i++) {
        pthread_mutex_lock(&counterMutex);
        while (counter != NO_REINDEERS) {
            pthread_cond_wait(&counterCond, &counterMutex);
        }
        pthread_mutex_unlock(&counterMutex);
        printf("Mikołaj: budzę się\n");
        printf("Mikołaj: dostarczam zabawki\n");
        struct timespec ts = {
                .tv_sec = rand() % 2 + 2,
                .tv_nsec = random() % 1000L * 1000000L
        };
        nanosleep(&ts, NULL);
        printf("Mikołaj: zasypiam\n");
        pthread_mutex_lock(&counterMutex);
        counter = 0;
        pthread_mutex_unlock(&counterMutex);
        pthread_cond_broadcast(&counterCond);
    }
    return NULL;
}

void* reindeer(void* arg) {
    int id = *((int*) arg);
    for (int i = 0; i < NO_YEARS; i++) {
        pthread_mutex_lock(&counterMutex);
        while (counter != 0) {
            pthread_cond_wait(&counterCond, &counterMutex);
        }
        pthread_mutex_unlock(&counterMutex);
        struct timespec ts = {
                .tv_sec = rand() % 5 + 5,
                .tv_nsec = random() % 1000L * 1000000L
        };
        nanosleep(&ts, NULL);
        pthread_mutex_lock(&counterMutex);
        counter++;
        printf("Renifer: czeka %d reniferów na Mikołaja, ID: %d\n", counter, id);
        pthread_mutex_unlock(&counterMutex);
        pthread_cond_broadcast(&counterCond);
    }
    return NULL;
}

int main(void) {
    pthread_mutex_init(&counterMutex, NULL);
    pthread_cond_init(&counterCond, NULL);

    srand(time(NULL));
    srandom(time(NULL));
    int** ids = malloc(sizeof(int*) * NO_REINDEERS);

    pthread_t santa_thread;
    if (pthread_create(&santa_thread, NULL, &santa, NULL) != 0)
        perror("Error creating the santa thread");
    pthread_t reindeer_threads[NO_REINDEERS];
    for (int i = 0; i < NO_REINDEERS; i++) {
        ids[i] = malloc(sizeof(int));
        *ids[i] = i + 1;
        if (pthread_create(&reindeer_threads[i], NULL, &reindeer, ids[i]) != 0)
            perror("Error creating a reindeer thread");
    }

    if (pthread_join(santa_thread, NULL) != 0)
        perror("Error joining the santa thread");
    for (int i = 0; i < NO_REINDEERS; i++) {
        free(ids[i]);
        if (pthread_join(reindeer_threads[i], NULL) != 0)
            perror("Error joining the santa thread");
    }

    free(ids);
    pthread_mutex_destroy(&counterMutex);
    pthread_cond_destroy(&counterCond);

    return 0;
}
