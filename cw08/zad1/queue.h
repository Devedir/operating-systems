#define TEXT_LEN 11 // including '\0'
#define QUEUE_CAPACITY 2

typedef struct queue_t {
    int size;
    int before;
    char body[QUEUE_CAPACITY][TEXT_LEN];
} queue_t;

queue_t q_init();

void q_push(queue_t* queue, char text[TEXT_LEN]);

char* q_pop(queue_t* queue);
