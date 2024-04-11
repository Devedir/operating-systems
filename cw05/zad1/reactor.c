#define _POSIX_SOURCE 700

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

void handler(int sig) {
    printf("Signal received!\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "One argument required: signal reaction type.\n");
        return 1;
    }

    bool should_check_after = false;
    char* reaction_type = argv[1];
    if (strcmp(reaction_type, "ignore") == 0)
        signal(SIGUSR1, SIG_IGN);
    else if (strcmp(reaction_type, "handler") == 0)
        signal(SIGUSR1, handler);
    else if (strcmp(reaction_type, "mask") == 0) {
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
            perror("Signal blocking unsuccessful.");
            return 3;
        }
        should_check_after = true;
    }
    else if (strcmp(reaction_type, "none") != 0) {
        fprintf(stderr, "Illegal argument - use 'none', 'ignore', 'handler' or 'mask'.\n");
        return 2;
    }

    raise(SIGUSR1);

    if (should_check_after) {
        sigset_t pending;
        sigpending(&pending);
        if (sigismember(&pending, SIGUSR1))
            printf("I can see a user signal pending.\n");
    }

    return 0;
}
