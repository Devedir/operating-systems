#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define DONT_DIE (-1)
#define MAX_MESSAGE_LEN 256

int try_n_die(int value, int avoid, const char* message, int code);
