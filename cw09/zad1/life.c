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
    grids_t* grids = info->si_value.sival_ptr;
    update_grid(grids->background, grids->foreground);
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

	char *tmp;
    grids_t grids = {
            .foreground = create_grid(),
            .background = create_grid()
    };

	init_grid(grids.foreground);

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
            return 3;
        }
    }
    pthread_attr_destroy(&attr);

    struct sigaction action;
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGALRM);
    action.sa_mask = mask;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = alarm_handler;
    sigaction(SIGALRM, &action, NULL);
    union sigval sig_grid = {
            .sival_ptr = &grids
    };

	while (true)
	{
		draw_grid(grids.foreground);
        for (int i = 0; i < n; i++) {
            pthread_sigqueue(thread_ids[i], SIGALRM, sig_grid);
        }
		usleep(500 * 1000);

		tmp = grids.foreground;
        grids.foreground = grids.background;
        grids.background = tmp;
	}

    free(thread_ids);

	endwin(); // End curses mode
	destroy_grid(grids.foreground);
	destroy_grid(grids.background);

	return 0;
}
