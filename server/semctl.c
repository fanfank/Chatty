//void Pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *attr attr);
//void Pthread_mutex_destroy(pthread_mutex_t *mutex);
//void Sem_init(sem_t *, int, unsigned int);
//void Sem_destroy(sem_t *);
#include "header.h"

void
Sem_init(sem_t *sem, int shared, unsigned int value)
{
    if(sem_init(sem, shared, value) != 0) 
        _err_quit("sem_init error", -1);
}

void
Sem_destroy(sem_t *sem)
{
    if(sem_destroy(sem) != 0)
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
    if(sem_getvalue(sem, val) != 0)
        _err_quit("sem_getvalue error", -1);
}

void
Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    int ec;
    if((ec = pthread_mutex_init(mutex, attr)) != 0)
        _err_quit("phtread_mutex_init error", ec);
}

void
Pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    int ec;
    if((ec = pthread_mutex_destroy(mutex)) != 0)
        _err_quit("pthread_mutex_destroy error", ec);
}

void
Pthread_mutex_lock(pthread_mutex_t *mutex)
{
    int ec;
    if((ec = pthread_mutex_lock(mutex)) != 0)
        _err_quit("pthread_mutex_lock error", ec);
}

void
Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    int ec;
    if((ec = pthread_mutex_unlock(mutex)) != 0)
        _err_quit("pthread_mutex_unlock error", ec);
}

void
Pthread_rwlock_init(pthread_rwlock_t *rwlock)
{
    int ec;
    if((ec = pthread_rwlock_init(rwlock, 0)) != 0)
    {
        _err_quit("Init rwlock failed", ec);
    }
}

void
Pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
    int ec;
    if((ec = pthread_rwlock_destroy(rwlock)) != 0)
    {
        _err_quit("Destroy rwlock failed", ec);
    }
}

void
Pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    int ec;
    if((ec = pthread_rwlock_wrlock(rwlock)) != 0)
    {
        printf("%s\n", strerror(ec));
        _err_quit("Request for write lock failed", ec);
    }
}

void
Pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    int ec;
    if((ec = pthread_rwlock_rdlock(rwlock)) != 0)
    {
        printf("%s\n", strerror(ec));
        _err_quit("Request for read lock failed", ec);
    }
}

void
Pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    int ec;
    if((ec = pthread_rwlock_unlock(rwlock)) != 0)
    {
        printf("%s\n", strerror(ec));
        _err_quit("Rwlock unlock failed", ec);
    }
}
