#include "header.h"
#include "semctl.h"
#include "connectserver.h"
#include "communicate.h"
#include "pressuretest.h"
#define DFLSERVERADDR "10.42.0.1"
#define DFLSERVERPORT "4001"
#define DFLSERVERNAME "server"
extern void checkInRange(int *num, int range_bg, int range_ed);

struct addrinfo local_info, chatmate_info, server_info;
sem_t *client_thread_sem;

void printWelcomeMsg();
void runClient();
void initAddr(struct addrinfo *, struct addrinfo *);
void copyAddr(struct addrinfo *, struct addrinfo *);

extern void pressureTest();

int
main(int argc, char *argv[])
{
    srand((uint)time(0));
    while(1)
    {
        printWelcomeMsg();
        int cmd;
        checkInRange(&cmd, 1, 2);
        switch(cmd)
        {
            case 1:
                runClient();
                break;
            case 2:
                printf("------Good Bye!------\n");
                exit(0);
                break;
            default:
                printf("Unknown command, please choose between 1 ~ 2\n");
                break;
        }
    }
}

void
printWelcomeMsg()
{
    printf("------- Welcome to Chatty -------\n");
    //printf("1 Choose available chatmates\n");
    printf("1 set addresses and connect\n");
    printf("2 quit\n");
    printf("-------Pls Choose a number-------\n");
}

void
runClient()
{
    struct addrinfo info = {DFLSERVERADDR, DFLSERVERPORT, DFLSERVERNAME};
    printf("------ Setting server ------\n");
    initAddr(&server_info, &info);

    client_thread_sem = Sem_init(client_thread_sem, 0, 1);
    while(1)
    {
        char c[BUFFLEN];
        printf("Do you want to do the pressure test?(y/n)");
        scanf("%s", c);
        while(c[0] != 'y' && c[0] != 'n')
        {  
            printf("Unknown command\n");
            scanf("%s", c);
        }
        if(c[0] == 'n')
            break;

        pressureTest();
    }

    printf("\nSkipped pressure test, start chatting ... \n");
    int server_fd = connectServer(server_info.address, server_info.port);
    if(server_fd == -1)
    {
        printf("Connection failed, pls try again later\n");
        return;
    }

    communicate(server_fd);
    Sem_destroy(client_thread_sem);
}

void
initAddr(struct addrinfo *dst, struct addrinfo *src)
{
    printf("Do you want to use the default settings:\n");
    printf("Address: %s:%s\n(y/n)", src->address, src->port);
    char sel[BUFFLEN] = "0";
    scanf("%s", sel);
    while(sel[0] != 'y' && sel[0] != 'n')
    {
        printf("Unknown input, please enter 'y' or 'n'\n");
        scanf("%s", sel);
    }

    if(sel[0] == 'y')
        copyAddr(dst, src);
    else
    {
        int len;
        int f = -1;
        while(f == -1)
        {
            printf("Pls enter a valid Address: ");
            scanf("%s",src->address);
            //f = validateAddress(src->address);
            f = 0;
        }
        len = (int)strlen(src->address);
        if(src->address[len] != 0)
            src->address[len] = 0;
        
        f = -1;
        while(f == -1)
        {
            printf("Pls enter a valid port number: ");
            scanf("%s", src->port);
            //f = validatePort(src->port);
            f = 0;
        }
        len = (int)strlen(src->port);
        if(src->port[len] != 0)
            src->port[len] = 0;

        copyAddr(dst, src);
    }
}

void
copyAddr(struct addrinfo *dst, struct addrinfo *src)
{
    strcpy(dst->address, src->address);
    strcpy(dst->port, src->port);
    strcpy(dst->name, src->name);
}
