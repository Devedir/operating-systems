#include "common.h"

#define MAX_NO_CLIENTS 5

int main(void) {
    const int server_sock = try_n_die(socket(AF_INET, SOCK_STREAM, 0), -1,
                                "Error creating the server socket", 1);
    const struct in_addr internet_addr = {
            .s_addr = INADDR_ANY
    };
    const struct sockaddr_in server_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(1800),
            .sin_addr = internet_addr
    };
    try_n_die(bind(server_sock, (struct sockaddr*) &server_addr, sizeof server_addr), -1,
                "Error binding the server socket", 2);
    printf("Server IP: %d\n", server_addr.sin_addr.s_addr);
    printf("Server port: %d\n", ntohs(server_addr.sin_port));
    try_n_die(listen(server_sock, MAX_NO_CLIENTS), -1,
                "Error starting to listen on a server", 3);
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof client_addr;
    try_n_die(accept(server_sock, (struct sockaddr*) &client_addr, &client_len), -1,
                "Error starting to accept connections", 4);

    // TODO: recv and send

    try_n_die(shutdown(server_sock, SHUT_RDWR), -1,
                "Error shutting down the server socket", 7);
    try_n_die(close(server_sock), -1,
                "Error closing server socket file descriptor", 8);

    return 0;
}