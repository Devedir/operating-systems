#include <mqueue.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char name[18];
    snprintf(name, 18, "/chat-client-%d", getpid());
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
    char message[22];
    strcat(message, "INIT");
    strcat(message, name);
    int sending = mq_send(server_queue, message, 22, 2);
    if (sending == -1) {
        perror("Server queue sending error");
        return 3;
    }
    int closing = mq_close(server_queue);
    if (closing == -1) {
        perror("Server queue closing error");
        return 4;
    }

    closing = mq_close(my_queue);
    if (closing == -1) {
        perror("Client queue closing error");
        return 5;
    }
    int unlinking = mq_unlink(name);
    if (unlinking == -1) {
        perror("Client queue unlinking error");
        return 6;
    }

    return 0;
}
