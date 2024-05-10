#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

void parse_args(int argc, char* argv[], double* h, int* n) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments required: rectangle width and number of them.\n");
        exit(-1);
    }

    errno = 0;
    *h = strtod(argv[1], NULL);
    if (errno != 0) {
        perror("Incorrect rectangle width");
        exit(1);
    }
    *n = (int) strtol(argv[2], NULL, 10);
    if (errno != 0) {
        perror("Incorrect number of rectangles");
        exit(2);
    }
}

int main(int argc, char* argv[]) {
    double h; int n;
    parse_args(argc, argv, &h, &n);

    // Read the boundaries
    double boundaries[2] = {0};
    printf("Give the boundaries of integration: ");
    scanf("%lf %lf", &boundaries[0], &boundaries[1]);

    // Send calculation parameters to the integrator through a named pipe
    if (mkfifo("calc_data.fifo", S_IRWXU) != 0) {
        if (errno != EEXIST) {
            perror("Error creating a fifo");
            return 10;
        }
    }
    FILE* fifo = fopen("calc_data.fifo", "wb");
    if (fifo == NULL) {
        perror("Pipe opening error");
        return 11;
    }
    if (fwrite(&h, sizeof h, 1, fifo) != 1) return 12;
    if (fwrite(&n, sizeof n, 1, fifo) != 1) return 13;
    if (fwrite(boundaries, sizeof boundaries[0], 2, fifo) != 2) return 14;
    if (fclose(fifo) != 0) {
        perror("Pipe closing error");
        return 15;
    }

    // Wait and read the results from another pipe
    double result;
    fifo = fopen("calc_data.fifo", "rb");
    if (fifo == NULL) {
        perror("Pipe opening error");
        return 20;
    }
    if (fread(&result, sizeof result, 1, fifo) != 1) return 21;
    if (fclose(fifo) != 0) {
        perror("Pipe closing error");
        return 22;
    }

    // Print those results
    printf("Result: %lf\n", result);
    return 0;
}
