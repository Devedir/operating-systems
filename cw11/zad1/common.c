#include "common.h"

int try_n_die(int value, int avoid, const char* message, int code) {
    if (value == avoid) {
        perror(message);
        if (code != DONT_DIE)
            exit(code);
    }
    return value;
}
