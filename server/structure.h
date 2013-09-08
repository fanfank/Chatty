#include "macros.h"
struct addrinfo
{
    char address[BUFFLEN];
    char port[10];
    char name[BUFFLEN];
};

struct hashtable {
    //sem_t mutex;
    int magic;
    int fd;
    int ready;
    struct hashtable *pre;
    struct hashtable *next;
    char addr[HTADDRLEN];
    char port[HTPORTLEN];
    char name[HTNAMELEN];
};

struct list {
    struct hashtable ht_head;
    struct hashtable ht_tail;
    pthread_rwlock_t htlist_rwlock;//Here you need to use rdwr lock
};
