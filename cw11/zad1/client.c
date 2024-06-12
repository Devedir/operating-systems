#include "common.h"

#include <errno.h>

void parse_args(int argc, char* argv[], char** name, int* server_ip, int* port) {
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments - pass a client name, server IP and port\n");
        exit(1);
    }
    errno = 0;
    char* endptr;
    *server_ip = (int) strtol(argv[2], &endptr, 10);
    if (errno != 0) {
        perror("IP parsing error");
        exit(2);
    }
    if (argv[3] == endptr) {
        fprintf(stderr, "IP parsing error: No digits found\n");
        exit(2);
    }
    *port = (int) strtol(argv[3], &endptr, 10);
    if (errno != 0) {
        perror("Port number parsing error");
        exit(3);
    }
    if (argv[3] == endptr) {
        fprintf(stderr, "Port number parsing error: No digits found\n");
        exit(3);
    }
    *name = argv[1];
}

int main(int argc, char* argv[]) {
    char* name;
    int server_ip, port;
    parse_args(argc, argv, &name, &server_ip, &port);

    const int client_sock = try_n_die(socket(AF_INET, SOCK_STREAM, 0), -1,
                                        "Error creating the client socket", 1);
    const struct in_addr internet_addr = {
            .s_addr = server_ip
    };
    const struct sockaddr_in server_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = internet_addr
    };
    try_n_die(connect(client_sock, (struct sockaddr*) &server_addr, sizeof server_addr), -1,
                "Error connecting to the server", 2);

    // TODO: recv and send

    try_n_die(shutdown(client_sock, SHUT_RDWR), -1,
              "Error shutting down the server socket", 7);
    try_n_die(close(client_sock), -1,
              "Error closing server socket file descriptor", 8);

    return 0;
}
