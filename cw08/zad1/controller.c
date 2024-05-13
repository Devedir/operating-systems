#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "queue.h"

void parse_input(int argc, char* argv[], int* no_users, int* no_printers) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments required: number of users and printers.\n");
        exit(1);
    }

    errno = 0;
    char* endptr = argv[1];
    *no_users = (int) strtol(argv[1], &endptr, 10);
    if (argv[1] == endptr) {
        fprintf(stderr, "The first argument is not a valid number.\n");
        exit(2);
    }
    *no_printers = (int) strtol(argv[2], &endptr, 10);
    if (argv[2] == endptr) {
        fprintf(stderr, "The second argument is not a valid number.\n");
        exit(2);
    }
    if (errno != 0) {
        perror("Arguments parsing error");
        exit(3);
    }
    if (*no_users <= 0 || *no_printers <= 0) {
        fprintf(stderr, "Numbers must be positive.\n");
        exit(4);
    }
}

int main(int argc, char* argv[]) {
    int no_users, no_printers;
    parse_input(argc, argv, &no_users, &no_printers);

    queue_t queue = q_init();
    q_push(&queue, "some_word1");
    q_push(&queue, "some_word2");
    q_push(&queue, "some_word3");
    char* str = q_pop(&queue);
    if (str == NULL) {
        fprintf(stderr, "Queue is empty!\n");
    } else printf("%s\n", str);
    str = q_pop(&queue);
    if (str == NULL) {
        fprintf(stderr, "Queue is empty!\n");
    } else printf("%s\n", str);
    str = q_pop(&queue);
    if (str == NULL) {
        fprintf(stderr, "Queue is empty!\n");
    } else printf("%s\n", str);

    return 0;
}
