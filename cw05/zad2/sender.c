#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments required: catcher PID and operating mode.\n");
        return 1;
    }
    errno = 0;
    pid_t catcher_pid = (pid_t) strtol(argv[1], NULL, 10);
    if (errno != 0) {
        perror("First argument is not a valid number");
        return 2;
    }
    int mode = (int) strtol(argv[2], NULL, 10);
    if (errno != 0) {
        perror("Second argument is not a valid number");
        return 3;
    }

    union sigval u_mode = {mode};
    if (sigqueue(catcher_pid, SIGUSR1, u_mode) != 0) {
        perror("Signal sending error");
        return 4;
    }

    sigset_t sigset;
    sigprocmask(0, NULL, &sigset);
    sigsuspend(&sigset);

    return 0;
}
