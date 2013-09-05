#include "header.h"
struct hashtable table[MAXFD];
extern void t(char *, int);
void
initHashtable(struct hashtable ht[], int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        Sem_init(&(ht[i].mutex), 0, 1);
        ht[i].magic = 0;
    }
}

void
destroyHashtable(struct hashtable ht[], int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        Sem_destroy(&(ht[i].mutex));
    }
}

int
hashAdd(int num)
{
    int key = num % MAXFD;
    int cnt = MAXFD;
    while(cnt--)
    {
        //t("hash key in:", key);
        Sem_wait(&(table[key].mutex));
        if(table[key].magic != MAGICNUM)
        {
            table[key].magic = MAGICNUM;
            Sem_post(&(table[key].mutex));
            return key;
        }
        Sem_post(table[key].mutex);
        key = (++key) % MAXFD;
    }
    return -1;
}

int 
hashFind(int num)
{
    int key = num % MAXFD;
    while(1)
    {
        Sem_wait(&(table[key].mutex));
        if(table[key].magic == MAGICNUM && table[key].fd == num)
        {
            Sem_post(&(table[key].mutex));
            break;
        }
        Sem_post(&(table[key].mutex));
        key = (++key) % MAXFD;
    }
    return key;
}

int
addToTable(int fd, struct sockaddr_in *addr)
{
    int key = hashAdd(fd);
    //t("hash key:", key);
    if(key < 0)
    {
        return -1;
    }
    table[key].fd = fd;
    strcpy(table[key].addr, inet_ntoa(addr->sin_addr));
    sprintf(table[key].port, "%d", ntohs(addr->sin_port));
    sprintf(table[key].name, "Client%d", fd);
    return key;
}

void
removeFromTable(int fd)
{
    int key = hashFind(fd);
    table[key].magic = 0;
}
