#include "common.h"

#include <time.h>

#define MAX_NO_CLIENTS 5

typedef struct client {
    char name[MAX_NAME_LEN];
    int sockfd;
    bool active;
    pthread_mutex_t mutex;
} client;

client clients[MAX_NO_CLIENTS] = {0};
int no_clients = 0;

void* handle_client(void* arg) {
    pthread_detach(pthread_self());
    const int client_num = *((int*) arg);
    free(arg);
    time_t timer = time(NULL);
    client* self = &clients[client_num];
    char read_buf[MAX_MESSAGE_LEN] = {0};
    char write_buf[MAX_FULL_LEN] = {0};
    while (true) {
        ssize_t n = try_n_die(read(self->sockfd, read_buf, MAX_MESSAGE_LEN), -1,
                              "Error reading from socket", DONT_DIE);
        time_t now = time(NULL);
        if (n == 0 || strncmp(read_buf, "STOP", 4) == 0) {
            printf("%s left the chat\n", self->name);
            pthread_mutex_lock(&self->mutex);
            self->active = false;
            pthread_mutex_unlock(&self->mutex);
            return NULL;
        }
        else if (read_buf[0] == '2') {
            struct tm* local = localtime(&now);
            if (strncmp(read_buf, "2ALL", 4) == 0) {
                snprintf(write_buf, MAX_FULL_LEN, "%s (%d-%d-%d): %s",
                         self->name, local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, read_buf + 5);
                for (int i = 0; i < no_clients; i++) {
                    pthread_mutex_lock(&clients[i].mutex);
                    if (clients[i].active == true && i != client_num) {
                        try_n_die(write(clients[i].sockfd, write_buf, MAX_FULL_LEN), -1,
                                  "Error writing on a sock", DONT_DIE);
                    }
                    pthread_mutex_unlock(&clients[i].mutex);
                }
            }
            else if (strncmp(read_buf, "2ONE", 4) == 0) {
                char* reciever_name = read_buf + 5;
                char* msg = reciever_name;
                while (*msg != ' ' && *msg != '\n' && *msg != '\0') msg++;
                if (*msg == ' ') {
                    *msg = '\0';
                    msg++;
                }
                snprintf(write_buf, MAX_FULL_LEN, "%s (%d-%d-%d): %s",
                         self->name, local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, msg);
                for (int i = 0; i < no_clients; i++) {
                    pthread_mutex_lock(&clients[i].mutex);
                    if (clients[i].active == true && strncmp(clients[i].name, reciever_name, MAX_NAME_LEN) == 0) {
                        try_n_die(write(clients[i].sockfd, write_buf, MAX_FULL_LEN), -1,
                                  "Error writing on a sock", DONT_DIE);
                    }
                    pthread_mutex_unlock(&clients[i].mutex);
                }
            }
        }
        else if (strncmp(read_buf, "LIST", 4) == 0) {
            char listing_buf[16 + MAX_NAME_LEN * MAX_NO_CLIENTS] = {0};
            snprintf(listing_buf, 17, "Active clients: ");
            char* ptr = listing_buf + 16;
            for (int i = 0; i < no_clients; i++) {
                if (clients[i].active) {
                    snprintf(ptr, MAX_NAME_LEN + 2, "%s, ", clients[i].name);
                    while (*ptr != '\0') ptr++;
                }
            }
            *(ptr - 2) = '\n';
            *(ptr - 1) = '\0';
            try_n_die(write(self->sockfd, listing_buf, 16 + MAX_NAME_LEN * MAX_NO_CLIENTS), -1,
                      "Error writing on a sock", DONT_DIE);
        }

        // Pinging at least 5 seconds apart
        if (time(NULL) - timer > 5) {
            try_n_die(write(self->sockfd, "PING", 5), -1,
                      "Error pinging", DONT_DIE);
            n = try_n_die(read(self->sockfd, read_buf, 6), -1,
                          "Error reading from socket", DONT_DIE);
            if (n == 0 || strncmp(read_buf, "ALIVE", 5) != 0) {
                printf("%s left the chat\n", self->name);
                self->active = false;
                return NULL;
            }
            timer = time(NULL);
        }
    }
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
        pthread_mutex_init(&clients[no_clients].mutex, NULL);
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
        pthread_mutex_destroy(&clients[i].mutex);
    }

    return 0;
}
