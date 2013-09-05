#include <semaphore.h>
#include <pthread.h>
#include "../macros.h"

struct queue_data {
    int fd;
    int is_read;
    char buf[BUFFLEN];
};
struct job_queue {
    int nput, nget;
    sem_t nempty, nstored;
    pthread_mutex_t mutex;
    struct queue_data data[MAXFD];
};

struct worker_queue {
    pthread_mutex_t mutex;
    pthread_t tid_worker[MAXWORKER];
    int nworker;
};
