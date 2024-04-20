#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void parse_args(int argc, char* argv[], double* h, int* n) {
    if (argc != 3) {
        fprintf(stderr, "Four arguments required: boundaries, rectangle width and number of them.\n");
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
    double a, b;
    printf("Give the boundaries of integration: ");
    scanf("%lf %lf", &a, &b);

    // Send calculation parameters to the integrator through a named pipe

    // Wait and read the results from another pipe

    // Print those results

    return 0;
}
