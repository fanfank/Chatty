#include "header.h"
#define DFLADDR INADDR_ANY
#define DFLPORT "4001"

int stop_working;

int initServer();
extern struct hashtable table[];
extern void *threadpoolStart(void *arg);

void t(char s[], int i){printf("%s %d\n", s, i);fflush(stdout);}

int
main(int argc, char *argv[])
{
    pthread_t tid_threadpool_manager;
    struct epoll_event ev;
    int sockfd = initServer();
    //t("here", 1);
    int epollfd = initEpoll(sockfd);

    initHashtable(&table, MAXFD);
    stop_working = -1;
    if(pthread_create(&tid_threadpool_manager, 0, threadpoolStart,
                0) < 0)
    {
        close(epollfd);
        close(sockfd);
        _err_quit("Create threadpool manager error", -1);
    }
    while(stop_working != 0)
        usleep(1000);

    runEpoll(epollfd, sockfd);

    stop_working = 1;
    close(sockfd);
    close(epollfd);
    destroyHashtable(&table, MAXFD);
    if(pthread_join(tid_threadpool_manager, 0) < 0)
    {
        _err_quit("Join threadpool manager error", -1);
    }
    printf("Server job done\n");
    exit(0);
}

int
initServer()
{
    int err;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        _err_quit("Create socket error", sockfd);

    int reuse = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse,
                sizeof(reuse)) < 0)
    {
        printf("setsockopt error: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(DFLPORT));
    addr.sin_addr.s_addr = htonl(DFLADDR);
    //inet_pton(AF_INET, DFLADDR, (void *)&addr.sin_addr);
    //t("here", 0);

    if((err = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))) < 0)
    {
        close(sockfd);
        _err_quit("Bind address error", err);
    }

    if((err = listen(sockfd, MAXFD)) < 0)
    {
        close(sockfd);
        _err_quit("Listen error", err);
    }

    return sockfd;
}
