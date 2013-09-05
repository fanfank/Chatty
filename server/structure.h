#include "macros.h"
struct addrinfo
{
    char address[BUFFLEN];
    char port[10];
    char name[BUFFLEN];
};

struct hashtable {
    sem_t mutex;
    int magic;
    int fd;
    char addr[20];
    char port[10];
    char name[BUFFLEN];
};
