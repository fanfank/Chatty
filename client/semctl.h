//
//  semctl.h
//  Chatty
//
//  Created by 徐瑞琦 on 13/10/4.
//  Copyright (c) 2013年 徐瑞琦. All rights reserved.
//

#ifndef Chatty_semctl_h
#define Chatty_semctl_h

#include <semaphore.h>

extern sem_t *Sem_init(sem_t *sem, int shared, unsigned int value);
extern void Sem_destroy(sem_t *sem);
extern void Sem_wait(sem_t *sem);
extern void Sem_post(sem_t *sem);
extern void Sem_getvalue(sem_t *sem, int *val);
extern void Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
extern void Pthread_mutex_destroy(pthread_mutex_t *mutex);
extern void Pthread_mutex_lock(pthread_mutex_t *mutex);
extern void Pthread_mutex_unlock(pthread_mutex_t *mutex);


#endif
