#include "common.h"

#include <errno.h>

void parse_args(int argc, char* argv[], char** name, int* server_ip, int* port) {
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments - pass a client name, server IP and port\n");
        exit(1);
    }
    if (strlen(argv[1]) >= MAX_NAME_LEN) {
        fprintf(stderr, "This client name is too long!\n");
        exit(2);
    }
    *name = argv[1];
    errno = 0;
    char* endptr;
    *server_ip = (int) strtol(argv[2], &endptr, 10);
    if (errno != 0) {
        perror("IP parsing error");
        exit(3);
    }
    if (argv[3] == endptr) {
        fprintf(stderr, "IP parsing error: No digits found\n");
        exit(3);
    }
    *port = (int) strtol(argv[3], &endptr, 10);
    if (errno != 0) {
        perror("Port number parsing error");
        exit(4);
    }
    if (argv[3] == endptr) {
        fprintf(stderr, "Port number parsing error: No digits found\n");
        exit(4);
    }
}

void* reading(void* arg) {
    pthread_detach(pthread_self());
    const int server_sock = *((int*) arg);
    char buf[MAX_FULL_LEN] = {0};
    while (true) {
        ssize_t n = try_n_die(read(server_sock, buf, MAX_FULL_LEN), -1,
                              "Error reading from socket", DONT_DIE);
        if (n == 0) {
            fprintf(stderr, "Server shut down\n");
            return NULL;
        }
        else if (strncmp(buf, "PING", 4) == 0) {
            try_n_die(write(server_sock, "ALIVE", 6), -1,
                      "Error writing on the sock", DONT_DIE);
        }
        else printf("%s", buf);
    }
}

int main(int argc, char* argv[]) {
    char* name;
    int server_ip, port;
    parse_args(argc, argv, &name, &server_ip, &port);

    const int server_sock = try_n_die(socket(AF_INET, SOCK_STREAM, 0), -1,
                                      "Error creating the server socket", 1);
    const struct in_addr internet_addr = {
            .s_addr = server_ip
    };
    const struct sockaddr_in server_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = internet_addr
    };
    try_n_die(connect(server_sock, (struct sockaddr*) &server_addr, sizeof server_addr), -1,
              "Error connecting to the server", 2);
    try_n_die(write(server_sock, name, MAX_NAME_LEN), -1,
              "Error writing on the sock", DONT_DIE);

    pthread_t reading_thread;
    int* arg = malloc(sizeof server_sock);
    *arg = server_sock;
    pthread_create(&reading_thread, NULL, &reading, arg);

    char buf[MAX_MESSAGE_LEN] = {0};
    while (strncmp(buf, "STOP", 4) != 0) {
        fgets(buf, MAX_MESSAGE_LEN, stdin);
        try_n_die(write(server_sock, buf, MAX_MESSAGE_LEN), -1,
                  "Error writing on the sock", DONT_DIE);
    }

    try_n_die(shutdown(server_sock, SHUT_RDWR), -1,
              "Error shutting down the server socket", 7);
    try_n_die(close(server_sock), -1,
              "Error closing server socket file descriptor", 8);

    return 0;
}
