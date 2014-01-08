#ifndef CONNECTSERVER_C
#define CONNECTSERVER_C
#include "header.h"
#include "semctl.h"
#define MAXSLEEP 16

extern sem_t *client_thread_sem;
int connectWait(int, struct sockaddr *, size_t);

int
connectServer(char addr[], char port[])
{
    printf("Connecting ... \n");
    int ec;

    printf("%d", (int)client_thread_sem);
    Sem_wait(client_thread_sem);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    Sem_post(client_thread_sem);

    if(sockfd < 0)
    {
        printf("Can't create socket\n");
        pthread_exit((void *)-1);
    }

    struct sockaddr_in server_addr;
    memset((void *)&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
    inet_pton(AF_INET, addr, (void *)&server_addr.sin_addr);

    if((ec = connectWait(sockfd, (struct sockaddr*)&server_addr, 
                    sizeof(server_addr))) < 0)
    {
        close(sockfd);
        return -1;
    }
    printf("Connection OK!\n");
    return sockfd;
}

int connectWait(int fd, struct sockaddr *addr, size_t size)
{
    int nsec;
    for(nsec = 1; nsec <= MAXSLEEP; nsec <<= 1)
    {
        if(connect(fd, addr, (uint)size) == 0)
            return 0;
        sleep(nsec);
    }
    return -1;
}
#endif
