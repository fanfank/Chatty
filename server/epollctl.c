#include "header.h"

extern struct hashtable table[];
extern void t(char *, int);
int
initEpoll(int sockfd)
{
    int err;
    int epollfd = epoll_create(MAXFD + 2);
    if(epollfd < 0)
        _err_quit("epoll create error", epollfd);

    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    
    if((err = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev)) < 0)
    {
        close(epollfd);
        close(sockfd);
        _err_quit("epoll_ctl in inition error", err);
    }

    return epollfd;
}

void
runEpoll(int epollfd, int sockfd)
{
    int i, nfds, client_addr_len = 1;
    struct sockaddr_in client_addr;
    struct epoll_event ev, events[MAXFD + 1];

    while(1)
    {
        if((nfds = epoll_wait(epollfd, events, MAXFD + 1, 
                        WAITINGTIME)) < 0)
        {
            _err_quit("epoll_wait error", nfds);
        }
        else if(nfds == 0)
            break;

        int available = MAXFD;
        for(i = 0; i < nfds; i++)
        {
            //a new client comes
            if(events[i].data.fd == sockfd)
            {
                if(available == 0)
                {
                    printf("No place for new client\n");
                    continue;
                }

                memset(&client_addr, 0, sizeof(client_addr));
                int clientfd = accept(sockfd, 
                        (struct sockaddr *)&client_addr, &client_addr_len);

                if(clientfd < 0)
                {
                    printf("Accept client error");
                    continue;
                }

                int key = addToTable(clientfd, &client_addr);
                if(key < 0)
                {
                    printf("Add to hashtable error\n");
                    continue;
                }

                printf("Connection from %s:%s\n", table[key].addr, 
                        table[key].port);

                ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                ev.data.fd = clientfd;
                if(epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd,
                            &ev) < 0)
                {
                    _err_quit("epoll_ctl clientfd error", -1);
                }
                available--;
            }

            else if(events[i].events & EPOLLRDHUP)
            {
                int clientfd = events[i].data.fd;
                removeFromTable(clientfd);
                close(clientfd);
                available++;
                printf("Client with fd = %d exits.\n", clientfd);
            }
            else if(events[i].events & EPOLLIN)
            {
                addJob(events[i].data.fd);
            }
        }
    }
}
