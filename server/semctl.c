//void Pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *attr attr);
//void Pthread_mutex_destroy(pthread_mutex_t *mutex);
//void Sem_init(sem_t *, int, unsigned int);
//void Sem_destroy(sem_t *);
#include "header.h"

void
Sem_init(sem_t *sem, int shared, unsigned int value)
{
    if(sem_init(sem, shared, value) < 0) 
        _err_quit("sem_init error", -1);
}

void
Sem_destroy(sem_t *sem)
{
    if(sem_destroy(sem) < 0)
        _err_quit("sem_init error", -1);
}

void
Sem_wait(sem_t *sem)
{
    if(sem_wait(sem) != 0)
        _err_quit("sem_wait error", -1);
}

void
Sem_post(sem_t *sem)
{
    if(sem_post(sem) != 0)
        _err_quit("sem_wait error", -1);
}

void
Sem_getvalue(sem_t *sem, int *val)
{
    if(sem_getvalue(sem, val) < 0)
        _err_quit("sem_getvalue error", -1);
}

void
Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    int ec;
    if((ec = pthread_mutex_init(mutex, attr)) < 0)
        _err_quit("phtread_mutex_init error", ec);
}

void
Pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    int ec;
    if((ec = pthread_mutex_destroy(mutex)) < 0)
        _err_quit("pthread_mutex_destroy error", ec);
}

void
Pthread_mutex_lock(pthread_mutex_t *mutex)
{
    int ec;
    if((ec = pthread_mutex_lock(mutex)) < 0)
        _err_quit("pthread_mutex_lock error", ec);
}

void
Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    int ec;
    if((ec = pthread_mutex_unlock(mutex)) < 0)
        _err_quit("pthread_mutex_unlock error", ec);
}
