#include <stdio.h>
#include "collatz.h"

#define MAX_ITER 10000

int main(void) {
    printf("Dla 2137: %d\n", test_collatz_convergence(2137, MAX_ITER));
    printf("Dla 321: %d\n", test_collatz_convergence(321, MAX_ITER));
    return 0;
}