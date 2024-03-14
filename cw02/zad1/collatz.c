#include "collatz.h"

int collatz_conjecture(int input) {
    if (input % 2 == 0) return input / 2;
    else return 3 * input + 1;
}

int test_collatz_convergence(int input, int max_iter) {
    int num = input;
    for (int i = 1; i <= max_iter; i++) {
        num = collatz_conjecture(num);
        if (num == 1)
            return i;
    }
    return -1;
}