#include <mqueue.h>

#define MESSAGE_SIZE 8192
#define QUEUE_NAME_LEN 18

typedef struct msg_t {
    char text[MESSAGE_SIZE];
    mqd_t id;
} msg_t;
