#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

double fun(double x) {
    return 4 / (x*x + 1);
}

int main(void) {
    // Wait and read the calculation parameters from a named pipe
    double h; int n;
    double boundaries[2] = {0};
    FILE* fifo = fopen("calc_data.fifo", "rb");
    if (fifo == NULL) {
        perror("Pipe opening error");
        return 1;
    }
    if (fread(&h, sizeof h, 1, fifo) != 1) return 2;
    if (fread(&n, sizeof n, 1, fifo) != 1) return 3;
    if (fread(&boundaries, sizeof boundaries[0], 2, fifo) != 2) return 4;
    if (fclose(fifo) != 0) {
        perror("Pipe closing error");
        return 5;
    }

    // Create n child processes to calculate a specific area
    int* read_ends = malloc(sizeof(int) * n);
    double x = boundaries[0];
    double dist = (boundaries[1] - boundaries[0]) / n;
    for (int i = 0; i < n; i++) {
        int pipedes[2];
        if (pipe(pipedes) == -1) {
            perror("Error creating a pipe");
            free(read_ends);
            return 10;
        }
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Error on forking nr %d: %s", i, strerror(errno));
            free(read_ends);
            return 11;
        }
        else if (pid == 0) {
            close(pipedes[0]);
            free(read_ends);
            double area = h * fun(x);
            if (write(pipedes[1], &area, sizeof area) == -1) return 1;
            return 0;
        }
        close(pipedes[1]);
        read_ends[i] = pipedes[0];
        x += dist;
    }

    // Wait and read their results summing them up
    free(read_ends);

    // Send back the result through a named pipe

    return 0;
}
