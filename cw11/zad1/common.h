#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#define DONT_DIE (-1)
#define MAX_NAME_LEN 32
#define MAX_MESSAGE_LEN 256
#define MAX_FULL_LEN (MAX_NAME_LEN + 15 + MAX_MESSAGE_LEN)

int try_n_die(int value, int avoid, const char* message, int code);
