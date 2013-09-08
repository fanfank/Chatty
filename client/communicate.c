#include "header.h"

extern void checkInRange(int *, int, int);
void setMode(int *);
int setBufHead(char buf[], const int mode, int *head_len, int fd);
int setChatmate(char buf[], int fd);
void startChatting(char buf[], int mode, int head_len, int fd);
void *threadRead(void *arg);
void printSentMsg(char buf[]);
void printReceivedMsg(char buf[]);

void
communicate(int server_fd)
{
    printf("------ Welcome ------");
    while(1)
    {
        int mode;
        int head_len;
        char buf[BUFFLEN];
        setMode(&mode);
        if(mode == 4)
            break;
        if(setBufHead(buf, mode, &head_len, server_fd) < 0)
            continue;
        write(server_fd, "0\n\0", 4); //tell the server that "i am ready"
        startChatting(buf, mode, head_len, server_fd);
        write(server_fd, "9\n\0", 4); //tell the server that "i am not ready"
    }
    close(server_fd);
}

void
setMode(int *mode)
{
    int f = -1;
    while(f == -1)
    {
        printf("Please Set a mode:\n");
        printf("1 echo back\n");
        printf("2 Specify a receiver\n");
        printf("3 Broadcast to all online clients\n");
        printf("4 Go Back\n");
        checkInRange(&f, 1, 4);
    }
    *mode = f;
}

int
setBufHead(char buf[], const int mode, int *head_len, int fd)
{
    switch(mode)
    {
        case 1:
            wrapHead1(buf, head_len);
            break;

        case 2:
            if(wrapHead2(buf, head_len, fd) == 0)
                return -1;
            break;

        case 3:
            wrapHead3(buf, head_len);
            break;

        default:
            break;
    }
    return 0;
}

void startChatting(char head[], int mode, int head_len, int fd)
{
    printf("*** Start Chatting, enter \"quit\" to return ***\n");
    pthread_t tid_read;
    if(pthread_create(&tid_read, 0, threadRead, (void *)&fd) < 0)
    {
        printf("Create read thread error, return\n");
        return;
    }

    char buf[BUFFLEN];
    const int max_input_len = BUFFLEN - 1 - head_len;
    setbuf(stdin, 0);
    while(1)
    {
        int i = 0;
        while(i < max_input_len)
        {
            buf[i] = getc(stdin);
            if(buf[i] == '\n')
                break;
            i++;
        }
        buf[i] = 0;

        if(strcmp("quit", buf) == 0)
            break;

        wrap(buf, head);
        write(fd, buf, strlen(buf) + 1);
        printSentMsg(&buf[head_len]); //just prints time
    }
    pthread_cancel(tid_read);
}

void *threadRead(void *arg)
{
    char buf[BUFFLEN];
    int n;
    int fd = *((int *)arg);
    while(1)
    {
        n = read(fd, buf, BUFFLEN);
        if(n < 0)
        {
            printf("Read from server error\n");
            break;
        }

        /*
         * A function that tests shared memory IPC
         * No effects on this program's functionality
         */
        sharedMemoryTest();

        printReceivedMsg(buf);
    }
    //pthread_exit((void *)0);
}
        
void printSentMsg(char buf[])
{
    //struct tm t = gmtime(time(0));
    time_t t = time(0);
    char tmp[BUFFLEN];
    strftime(tmp, BUFFLEN - 1, "%c", gmtime(&t));
    printf("Sent - %s\n\n", tmp);
}

void printReceivedMsg(char buf[])
{
    
    //printf("******** Received ********\n");
    //printf("You said:\n");
    time_t t = time(0);
    char tmp[BUFFLEN];
    strftime(tmp, BUFFLEN - 1, "%c", gmtime(&t));
    printf("%s - %s\n\n", buf, tmp);
    //printf("******** Received ********\n");
}
