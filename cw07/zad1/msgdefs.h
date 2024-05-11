#include <mqueue.h>

#define MESSAGE_SIZE 8192
#define QUEUE_NAME_LEN 18

// for client -> server communication
typedef struct msg_t {
    char text[MESSAGE_SIZE - sizeof(mqd_t)];
    mqd_t id;
} msg_t;
