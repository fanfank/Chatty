#ifndef HEADER
#define HEADER
#define BUFFLEN 256
#define MAX 0x7fffffff

#include <arpa/inet.h>
#include <errno.h>
#include <memory.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

struct addrinfo
{
    char address[BUFFLEN];
    char port[10];
    char name[BUFFLEN];
    char fd[8];
};

#endif
