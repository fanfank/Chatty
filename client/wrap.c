/*
 * this file defines functions that translate msg into a way that
 * the server understands
 * wrapHead1 means ask the server to echo back
 * wrapHead2 asks the server to send to a specific client
 * wrapHead3 asks the server to broadcast
 */
#ifndef WRAP_C
#define WRAP_C
#include "header.h"
#define MAXDISPLAY 20

extern struct addrinfo chatmate_info;
extern void checkInRange(int *num, int range_bg, int range_ed);
void checkInRange(int *sel, int range_bg, int range_end);
char *getContent(char *dst, char *src, char delimiter);
/*
 * A temp testing function 
 */
char *
wrap(char buf[], char head[])
{
    if(buf == NULL)
        return buf;
    char tmp[BUFFLEN];
    strcpy(tmp, buf);
    strcpy(buf, head);
    strcpy(&buf[strlen(head)], tmp);
    return buf;
}

char *
wrapHead1(char buf[], int *head_len)
{
    strcpy(buf, "1\n");
    buf[2] = 0;
    *head_len = 2;
    return buf;
}

char *
wrapHead2(char buf[], int *head_len, int fd)
{
    int n;
    buf[0] = '4';
    buf[1] = '\n';
    buf[2] = 0;
    write(fd, buf, 3);

    const int len = BUFFLEN + 50;
    char address[MAXDISPLAY][len];
    int addr_size = 0;

    printf("Select from the following addresses(index from 0):\n");
    while((n = read(fd, &address[addr_size], len - 1)) > 0)
    {
        if(address[addr_size][0] == 'f')
        {
            syslog(LOG_INFO, "Read f: %s\n", &address[addr_size]);
            break;
        }

        if(address[addr_size][n] != 0)
            address[addr_size][n] = 0;

        printf("#%d: %s", addr_size, &address[addr_size]);
        addr_size++;

        //bad design
        write(fd, "copied", 7);
    }
    if(n < 0)
    {
        printf("Get info from server error\n");
        return 0;
    }
    if(addr_size == 0)
    {
        printf("No chatmate available\n");
        printf("---------------------\n");
        return 0;
    }
    printf("------------------------\n");

    int sel;
    checkInRange(&sel, 0, addr_size - 1);

    char *pos;
    pos = &(address[sel][0]);
    pos = getContent(chatmate_info.address, pos, ' ');
    pos++;

    pos = getContent(chatmate_info.port, pos, ' ');
    pos++;

    pos = getContent(chatmate_info.name, pos, ' ');
    pos++;

    pos = getContent(chatmate_info.fd, pos, '\n');

    strcpy(buf, "2 ");
    int tmp_index = 2;
    int cp_size = strlen(chatmate_info.fd);
    strncpy(&buf[tmp_index], chatmate_info.fd, cp_size);
    buf[tmp_index + cp_size] = '\n';
    buf[tmp_index + cp_size + 1] = 0;
    
    *head_len = strlen(buf);
    return buf;
}

char *
wrapHead3(char buf[], int *head_len)
{
    strcpy(buf, "3\n");
    buf[2] = 0;
    *head_len = 2;
    return buf;
}

char *
getContent(char *dst, char *src, char delimiter)
{
    int i = 0;
    while(*(src + i) != delimiter)
    {
        *(dst + i) = *(src + i);
        i++;
    }
    *(dst + i) = 0;
    return (src + i);
}
#endif

