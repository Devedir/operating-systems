#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

bool dying = false;
int no_requests = 0;

void handler(int sig, siginfo_t *info, void *ucontext) {
    no_requests++;
    switch (info->si_value.sival_int) {
        case 1:
            for (int i = 1; i < 100; i++) {
                printf("%d, ", i);
                if (i % 10 == 0) printf("\n");
            }
            printf("%d\n", 100);
            break;
        case 2:
            printf("Number of requests: %d\n", no_requests);
            break;
        case 3:
            dying = true;
            break;
        default:
            printf("Unexpected operating mode: %d\n", info->si_value.sival_int);
    }
    kill(info->si_pid, SIGUSR1);
}

int main(void) {
    printf("catcher PID: %d\n", getpid());

    struct sigaction act;
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &act, NULL) != 0) {
        perror("Failed to change a signal action");
        return 1;
    }

    while (!dying) pause();

    return 0;
}
