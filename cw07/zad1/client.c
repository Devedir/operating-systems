#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "msgdefs.h"

int main(void) {
    char name[QUEUE_NAME_LEN];
    char pidbuff[11] = {'\0'};
    int pidlen = snprintf(pidbuff, 10, "%d", getpid());
    char pidstr[5];
    strncpy(pidstr, pidbuff + pidlen - 4, 5);
    snprintf(name, QUEUE_NAME_LEN, "/chat-client-%s", pidstr);

    mqd_t my_queue = mq_open(name, O_RDONLY | O_CREAT, 077777, NULL);
    if (my_queue == (mqd_t) -1) {
        perror("Client queue creating error");
        return 1;
    }
    mqd_t server_queue = mq_open("/chat-server", O_WRONLY);
    if (server_queue == (mqd_t) -1) {
        perror("Server queue opening error");
        return 2;
    }

    char message[MESSAGE_SIZE];
    strcat(message, "INIT");
    strcat(message, name);
    msg_t msg = {
            .text = "",
            .id = (mqd_t) -1
    };
    strncpy(msg.text, message, QUEUE_NAME_LEN + 4);
    int sending = mq_send(server_queue, (char*) &msg, sizeof(msg_t), 2);
    if (sending == -1) {
        perror("Server queue sending error");
        return 3;
    }

    mqd_t id;
    ssize_t receiving = mq_receive(my_queue, (char*) &id, MESSAGE_SIZE, NULL);
    if (receiving == (ssize_t) -1) {
        perror("Client queue receiving error");
        return 4;
    }

    int closing;

    if (id != (mqd_t) -1) {
        pid_t whoami = fork();
        if (whoami == -1) {
            perror("Error forking");
        } else if (whoami == 0) {
            closing = mq_close(server_queue);
            if (closing == -1)
                perror("Server queue closing error");

            while (1) {
                receiving = mq_receive(my_queue, message, MESSAGE_SIZE, NULL);
                if (receiving == (ssize_t) -1) {
                    perror("Client queue receiving error");
                    continue;
                }
                if (strncmp("CLOSE", message, 5) == 0) {
                    break;
                }
                printf("%s", message);
            }

            closing = mq_close(my_queue);
            if (closing == -1) {
                perror("Client queue closing error");
                return 6;
            }
        } else {
            closing = mq_close(my_queue);
            if (closing == -1)
                perror("Client queue closing error");

            msg.id = id;
            while (strncmp("CLOSE", msg.text, 5) != 0) {
                if (fgets(msg.text, TEXT_SIZE, stdin) == NULL) {
                    perror("Reading error");
                }
                sending = mq_send(server_queue, (char*) &msg, sizeof(msg_t), 1);
                if (sending == -1)
                    perror("Server queue sending error");
            }

            closing = mq_close(server_queue);
            if (closing == -1)
                perror("Server queue closing error");
            int unlinking = mq_unlink(name);
            if (unlinking == -1) {
                perror("Client queue unlinking error");
                return 7;
            }
        }
    }

    return 0;
}
