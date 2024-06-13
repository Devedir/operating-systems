#include "common.h"

#include <stdbool.h>
#include <pthread.h>
#include <time.h>

#define MAX_NO_CLIENTS 5

typedef struct client {
    char name[MAX_NAME_LEN];
    int sockfd;
    bool active;
} client;

client clients[MAX_NO_CLIENTS];
int no_clients = 0;

void* handle_client(void* arg) {
    pthread_detach(pthread_self());
    int client_num = *((int*) arg);
    free(arg);
    client* self = &clients[client_num];
    char buf[MAX_MESSAGE_LEN] = {0};
    time_t now;
    while (true) {
        ssize_t n = try_n_die(read(self->sockfd, buf, MAX_MESSAGE_LEN), -1,
                  "Error reading from socket", DONT_DIE);
        now = time(NULL);
        if (n == 0 || strncmp(buf, "STOP", 4) == 0) {
            printf("%s left the chat\n", self->name);
            self->active = false;
            break;
        }
        struct tm* local = localtime(&now);
        printf("%s (%d-%d-%d): %s", self->name, local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, buf);
    }
    return NULL;
}

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
    printf("Server port: %d\n\n", ntohs(server_addr.sin_port));
    try_n_die(listen(server_sock, MAX_NO_CLIENTS), -1,
              "Error starting to listen on a server", 3);

    struct sockaddr_in client_addr;
    socklen_t client_len;

    while (true) {
        clients[no_clients].sockfd = accept(server_sock, (struct sockaddr*) &client_addr, &client_len);
        if (clients[no_clients].sockfd == -1) {
            perror("Error accepting a connection");
            continue;
        }
        try_n_die(read(clients[no_clients].sockfd, clients[no_clients].name, MAX_NAME_LEN), -1,
                  "Error reading from socket", DONT_DIE);
        clients[no_clients].active = true;
        printf("%s joined the chat\n", clients[no_clients].name);
        pthread_t client_thread;
        int* curr_num = malloc(sizeof no_clients);
        *curr_num = no_clients;
        pthread_create(&client_thread, NULL, &handle_client, curr_num);
        no_clients++;
    }

    try_n_die(shutdown(server_sock, SHUT_RDWR), -1,
              "Error shutting down the server socket", 7);
    try_n_die(close(server_sock), -1,
              "Error closing server socket file descriptor", 8);
    for (int i = 0; i < no_clients; i++) {
        try_n_die(close(clients[i].sockfd), -1,
                  "Error closing client socket file descriptor", 9);
    }

    return 0;
}
