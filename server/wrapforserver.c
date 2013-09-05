#include "header.h"

extern struct hashtable table[];
char *
wrapHead1(char buf[], int fd)
{
    //Later
    return 0;
}

char *
addHead1(char buf[], int fd)
{
    int key = hashFind(fd);
    int n = sprintf(buf, "Echo back %s %s:%s\n", table[key].name, 
            table[key].addr, table[key].port);
    buf[n] = 0;
    return (buf + n);
}

char *
addHead2(char buf[], int fd)
{
    int key = hashFind(fd);
    int n = sprintf(buf, "*From %s %s %s*\n", table[key].name,
            table[key].addr, table[key].port);
    buf[n] = 0;
    return (buf + n);
}

char *
addHead4(char buf[], int key)
{
    int n = sprintf(buf, "%s %s %s %d\n", table[key].addr,
            table[key].port, table[key].name, table[key].fd);
    buf[n] = 0;
    return (buf + n);
}
