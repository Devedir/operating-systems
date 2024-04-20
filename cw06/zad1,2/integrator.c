#include <stdio.h>

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

    printf("Received: %lf %d %lf %lf", h, n, boundaries[0], boundaries[1]);

    // Create n subprocesses to calculate a specific area

    // Wait and read their results summing them up

    // Send back the result through a named pipe

    return 0;
}
