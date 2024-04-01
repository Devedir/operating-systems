#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int global = 0;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "One argument required: a directory path.\n");
        return 1;
    }
    printf("Hi, I'm %s\n", argv[0]);
    int local = 0;

    pid_t fork_pid = fork();
    if (fork_pid == -1) {
        perror("Error occurred on forking");
        exit(errno);
    } else if (fork_pid == 0) {
        printf("child process\n");
        global++; local++;
        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n", local, global);
        if (execl("/bin/ls", "ls", argv[1], NULL) == -1) {
            perror("Error occurred on executing /bin/ls");
            exit(errno);
        }
    } else {
        printf("parent process\n");
        printf("parent pid = %d, child pid = %d\n", getpid(), fork_pid);
        int status;
        wait(&status);
        printf("Child exit code: %d\n", WEXITSTATUS(status));
        printf("parent's local = %d, parent's global = %d\n", local, global);
        return WEXITSTATUS(status);
    }

    return 0;
}
