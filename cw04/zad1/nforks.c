#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "One argument required: number of child processes.\n");
        return 1;
    }

    int no_children = atoi(argv[1]);
    for (int i = 0; i < no_children; i++) {
        pid_t who = fork();
        if (who == 0) {
            printf("PPID: %d\tPID: %d\n", getppid(), getpid());
            exit(0);
        } else if (who == -1) {
            perror("Error occurred on forking");
            exit(errno);
        }
    }

    while (wait(NULL) != -1) {}
    printf("Number of children: %s\n", argv[1]);
    return 0;
}
