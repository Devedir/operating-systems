#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "msgdefs.h"

#define MAX_NO_CLIENTS 5

int main(void) {
    const char name[] = "/chat-server";
    mqd_t server_queue = mq_open(name, O_RDONLY | O_CREAT, 0777, NULL);
    if (server_queue == (mqd_t) -1) {
        perror("Queue creating error");
        return 1;
    }

    mqd_t client_ids[MAX_NO_CLIENTS];
    size_t registered_clients = 0;
    msg_t msg;
    int sending, closing;

    while (1) {
        ssize_t receiving = mq_receive(server_queue, (char*) &msg, sizeof(msg_t), NULL);
        if (receiving == (ssize_t) -1) {
            perror("Queue receiving error");
            break;
        }
        if (strncmp("CLOSE", msg.text, 5) == 0) {
            break;
        }
        if (strncmp("INIT", msg.text, 4) == 0) {
            char client_queue_name[QUEUE_NAME_LEN];
            strncpy(client_queue_name, msg.text + 4, QUEUE_NAME_LEN);
            mqd_t new_client_id = mq_open(client_queue_name, O_WRONLY);
            if (new_client_id == (mqd_t) -1) {
                perror("Client queue opening on server error");
                continue;
            }

            if (registered_clients == MAX_NO_CLIENTS) {
                fprintf(stderr, "Cannot register: Reached the maximum number of clients.\n");
                mqd_t closing_code = -1;
                sending = mq_send(new_client_id, (char*) &closing_code,
                                  sizeof(mqd_t), 3);
                if (sending == -1)
                    perror("Client queue sending error");
                closing = mq_close(new_client_id);
                if (closing == -1)
                    perror("Client queue closing error");
            } else {
                printf("New client: %s\n", client_queue_name);
                client_ids[registered_clients] = new_client_id;
                registered_clients++;

                sending = mq_send(new_client_id, (char*) &new_client_id,
                                  sizeof(mqd_t), 2);
                if (sending == -1)
                    perror("Client queue sending error");
            }
        } else {
            for (size_t i = 0; i < registered_clients; i++) {
                if (client_ids[i] == msg.id) continue;
                sending = mq_send(client_ids[i], msg.text, 6, 1);
                if (sending == -1)
                    perror("Client queue sending error");
            }
        }
    }

    for (size_t i = 0; i < registered_clients; i++) {
        sending = mq_send(client_ids[i], "CLOSE", 6, 3);
        if (sending == -1)
            perror("Client queue sending error");
        closing = mq_close(client_ids[i]);
        if (closing == -1)
            perror("Client queue closing on server error");
    }

    closing = mq_close(server_queue);
    if (closing == -1) {
        perror("Queue closing error");
        return 3;
    }
    int unlinking = mq_unlink(name);
    if (unlinking == -1) {
        perror("Queue unliking error");
        return 4;
    }

    return 0;
}
