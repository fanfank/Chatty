#include "../header.h"
//#define INITWORKER 10
//#define MAXWORKER 10

struct job_queue job;
struct worker_queue worker;

void *threadpoolStart(void *);
void *workerFetchesJob(void *arg);
void addWorker(int);
void delWorker(int);
void waitWorker();
extern void t(char s[], int i);

extern int stop_working;

void 
*threadpoolStart(void *arg)
{
    initJobQueue(&job, MAXFD);
    initWorkerQueue(&worker);

    addWorker(INITWORKER);
    stop_working = 0;
    waitWorker();

    destroyWorkerQueue(&worker);
    destroyJobQueue(&job);
    pthread_exit((void *)1);
}

void
addWorker(int num)
{
    int i;
    struct worker_queue *wk = &worker;

    Pthread_mutex_lock(&(wk->mutex));
    for(i = 0; i < num; i++)
    {
        if(wk->nworker == MAXWORKER)
        {
            printf("Too many workers, can't add\n");
            break;
        }
        pthread_create(&(wk->tid_worker[(wk->nworker)++]), 0,
                workerFetchesJob, 0);
        printf("Worker %d created\n", wk->nworker-1);
    }
    Pthread_mutex_unlock(&(wk->mutex));
}

void
delWorker(int num)
{
    //Later
}

void
waitWorker()
{
    int i;
    struct worker_queue *wk = &worker;

    Pthread_mutex_lock(&(wk->mutex));
    for(i = 0; i < wk->nworker; i++)
    {
        pthread_join(wk->tid_worker[i], 0);
        printf("Worker %d exited\n", i);
        (wk->nworker)--;
    }
    Pthread_mutex_unlock(&(wk->mutex));
}

void
addJob(int fd)
{
    Sem_wait(&(job.nempty));
    Pthread_mutex_lock(&(job.mutex));

    job.data[job.nput].fd = fd;
    job.data[job.nput].is_read = 1;
    job.data[job.nput].buf[0] = 0;
    if(++job.nput == MAXFD)
        job.nput = 0;

    Pthread_mutex_unlock(&(job.mutex));
    Sem_post(&(job.nstored));
    //t("Added one job", 1);
}

void *
workerFetchesJob(void *arg)
{
    struct queue_data data;
    while(1)
    {
        Sem_wait(&job.nstored);
        Pthread_mutex_lock(&job.mutex);
        if(stop_working)
        {
            Pthread_mutex_unlock(&job.mutex);
            Sem_post(&job.nstored);
            break;
        }

        workOnData(&(job.data[job.nget++]), &data);
        if(job.nget == MAXFD)
            job.nget = 0;

        Pthread_mutex_unlock(&job.mutex);
        Sem_post(&job.nempty);

        workOnIO(&data);
    }
    printf("Worker exiting ... \n");
    pthread_exit((void *)1);
}
