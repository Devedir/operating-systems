#include <mqueue.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>

#define MESSAGE_SIZE 8192

int main(void) {
    const char name[] = "/chat-server";
    mqd_t server_queue = mq_open(name, O_RDONLY | O_CREAT, 0777, NULL);
    if (server_queue == (mqd_t) -1) {
        perror("Queue creating error");
        return 1;
    }

    char message[MESSAGE_SIZE];
    ssize_t recieving = mq_receive(server_queue, message, MESSAGE_SIZE, NULL);
    if (recieving == (ssize_t) -1) {
        perror("Queue receiving error");
        return 2;
    }
    printf("%d:%s\n", server_queue, message);

    int closing = mq_close(server_queue);
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
