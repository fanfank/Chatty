#include "header.h"
struct hashtable table[MAXFD];
struct list htlist;
extern void t(char *, int);
int addToList(int);
int removeFromList(int);
void
initHashtable(struct hashtable ht[], int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        //Sem_init(&(ht[i].mutex), 0, 1);
        ht[i].magic = 0;
        ht[i].pre = ht[i].next = 0;
    }
    htlist.ht_head.fd = htlist.ht_tail.fd = 0;
    htlist.ht_head.pre = htlist.ht_tail.next = 0;
    htlist.ht_head.next = &htlist.ht_tail;
    htlist.ht_tail.pre = &htlist.ht_head;
    Pthread_rwlock_init(&htlist.htlist_rwlock);
}

void
destroyHashtable(/*struct hashtable ht[], int size*/)
{
    Pthread_rwlock_wrlock(&htlist.htlist_rwlock);
    struct hashtable *l = htlist.ht_head.next;
    while(l->next != 0)
    {
        memset(l->pre, 0, sizeof(struct hashtable));
        l = l->next;
    }
    memset(l, 0, sizeof(struct hashtable));
    l = 0;
    Pthread_rwlock_unlock(&htlist.htlist_rwlock);

    Pthread_rwlock_destroy(&htlist.htlist_rwlock);
}

int
hashAdd(int num)
{
    int key = num & MAXFD;
    int cnt = MAXFD;
    while(cnt--)
    {
        if(table[key].magic != MAGICNUM)
        {
            return key;
        }
        key = (++key) & MAXFD; //MAXFD must be 2^n - 1
    }
    return -1;
}

int 
hashFind(int num)
{
    int key = num & MAXFD;
    int cnt = MAXFD;
    while(cnt--)
    {
        if(table[key].magic == MAGICNUM && table[key].fd == num)
        {
            return key;
        }
        key = (++key) & MAXFD;
    }
    return -1;
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
    addToList(key); //magic is assigned here
    return key;
}

void
removeFromTable(int fd)
{
    int key = hashFind(fd);
    if(key < 0)
    {
        _err_quit("Error, can't remove fd, no key exists", -1);
    }
    removeFromList(key); //magic is destroyed here
}

int
addToList(int key)
{
    Pthread_rwlock_wrlock(&htlist.htlist_rwlock);
    //t("Read write", 1);
    table[key].pre = htlist.ht_tail.pre;
    table[key].next = &htlist.ht_tail;
    table[key].pre->next = table[key].next->pre = &table[key];
    table[key].magic = MAGICNUM;
    Pthread_rwlock_unlock(&htlist.htlist_rwlock);
    return 0;
}

int
removeFromList(int key)
{
    Pthread_rwlock_wrlock(&htlist.htlist_rwlock);
    table[key].magic = 0;
    table[key].pre->next = table[key].next;
    table[key].next->pre = table[key].pre;
    table[key].pre = table[key].next = 0;
    Pthread_rwlock_unlock(&htlist.htlist_rwlock);
    t("Client is removed with hash key", key);
    return 0;
}
