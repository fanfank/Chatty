#include "header.h"
#define MAXCLIENT 50
#define TESTROUND 10000
//#define PRINTALLMSG
pthread_t tid_threads[MAXCLIENT];

extern sem_t client_thread_sem;
extern struct addrinfo server_info;
void test(int);
void *threadStart(void *);

void
pressureTest()
{
    int bg_time = time(0);
    pthread_t tid_threads[MAXCLIENT];
    int i;
    for(i = 0; i < MAXCLIENT; i++) 
    {
        if(pthread_create(&tid_threads[i], 0, threadStart, 0) < 0)
        {
            printf("pthread_create error\n");
            break;
        }
        printf("Client %d created\n", i);
    }

    for(i = 0; i < MAXCLIENT; i++)
    {
        if(pthread_join(tid_threads[i], 0) < 0)
            _err_quit("can't join with thread", (-1)*i);
        printf("Thread %d exits\n", i);
    }
    Sem_destroy(client_thread_sem);
    int total_time = time(0) - bg_time;
    if(total_time == 0)
        total_time++;

    printf("--- Test Finished: %d secs %d IO/sec---\n\n", 
            total_time, MAXCLIENT*TESTROUND/total_time);
}

void *
threadStart(void *arg)
{
    int server_fd = connectServer(server_info.address, server_info.port);
    if(server_fd == -1)
        pthread_exit((void *)-1);
    
    test(server_fd);
    close(server_fd);
    pthread_exit((void *)0);
}

void
test(int fd)
{
    int n, cnt = TESTROUND;
    char buf[BUFFLEN];
    char res[BUFFLEN];
    int buf_size;
    while(cnt--)
    {
        n = sprintf(buf, "Hello, fd %d sent %d", fd, ((int)rand()) % MAX);
        buf[n] = 0;
        wrap(buf, "5 ");
        write(fd, buf, strlen(buf) + 1);
        n = read(fd, res, BUFFLEN);
        if(n < 0)
        {
            printf("Read error: %s\n", strerror(errno));
            return;
        }

        if(strcmp(buf, res) != 0)
        {
            printf("Receive incorrect: %s\n", res);
        }
#ifdef PRINTALLMSG
        else
            printf("Correct: %s\n", res);
#endif
    }
    return;
}
