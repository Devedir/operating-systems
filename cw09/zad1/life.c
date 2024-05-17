#define _GNU_SOURCE

#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

void alarm_handler(int signo, siginfo_t* info, void* extra) {
    updating_data_t* data = info->si_value.sival_ptr;
    update_grid(data);
}

void* updater(void* grids_ptr) {
    while (true) pause();
    return grids_ptr;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Pass one argument: number of threads.\n");
        return 1;
    }
    errno = 0;
    int n = (int) strtol(argv[1], NULL, 10);
    if (errno != 0) {
        perror("Argument parsing error");
        return 2;
    }

	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

    grids_t grids = {
            .foreground = create_grid(),
            .background = create_grid()
    };

	init_grid(grids.foreground);

    updating_data_t* updating_data = malloc(sizeof(updating_data_t) * n);
    int default_size = GRID_SIZE / n;
    int start_idx = 0;
    for (int i = 0; i < n; i++) {
        updating_data[i].grids = &grids;
        updating_data[i].start_idx = start_idx;
        updating_data[i].size = default_size;
        start_idx += default_size;
    }
    updating_data[n - 1].size += GRID_SIZE - (default_size * n);

    struct sigaction action;
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGALRM);
    action.sa_mask = mask;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = alarm_handler;
    sigaction(SIGALRM, &action, NULL);
    union sigval* sig_data = malloc(sizeof(union sigval) * n);
    for (int i = 0; i < n; i++) {
        sig_data[i].sival_ptr = &updating_data[i];
    }

    pthread_t* thread_ids = malloc(sizeof(pthread_t) * n);
    pthread_attr_t attr;
    for (int i = 0; i < n; i++) {
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        int creating = pthread_create(&thread_ids[i], &attr, &updater, &grids);
        if (creating != 0) {
            endwin(); // End curses mode
            perror("Error creating a thread");
            pthread_attr_destroy(&attr);
            destroy_grid(grids.foreground);
            destroy_grid(grids.background);
            free(thread_ids);
            free(updating_data);
            free(sig_data);
            return 3;
        }
    }
    pthread_attr_destroy(&attr);

	while (true) {
		draw_grid(grids.foreground);
        for (int i = 0; i < n; i++) {
            pthread_sigqueue(thread_ids[i], SIGALRM, sig_data[i]);
        }
		usleep(500 * 1000);

		char* tmp = grids.foreground;
        grids.foreground = grids.background;
        grids.background = tmp;
	}

    free(thread_ids);
    free(updating_data);
    free(sig_data);

	endwin(); // End curses mode
	destroy_grid(grids.foreground);
	destroy_grid(grids.background);

	return 0;
}
