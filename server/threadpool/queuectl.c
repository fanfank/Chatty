#include "../header.h"
void
initJobQueue(struct job_queue *q, int max_data_size)
{
    q->nput = q->nget = 0;
    Pthread_mutex_init(&(q->mutex), NULL);
    Sem_init(&(q->nempty), 0, max_data_size);
    Sem_init(&(q->nstored), 0, 0);
}

void
destroyJobQueue(struct job_queue *q)
{
    Pthread_mutex_destroy(&(q->mutex));
    Sem_destroy(&(q->nempty));
    Sem_destroy(&(q->nstored));
}

void
initWorkerQueue(struct worker_queue *q)
{
    Pthread_mutex_init(&(q->mutex), NULL);
    q->nworker = 0;
}

void
destroyWorkerQueue(struct worker_queue *q)
{
    Pthread_mutex_destroy(&(q->mutex));
}
