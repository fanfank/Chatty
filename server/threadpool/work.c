#include "../header.h"
#include <string.h>
#define MAXSTRSIZE 10000

void IORead(struct queue_data *data);
void IOWrite(struct queue_data *data);
extern char *addHead1(char buf[], int fd);
extern char *addHead2(char buf[], int fd);
extern char *addHead4(char buf[], int fd);
extern struct hashtable table[];
extern t(char *, int);

void
workOnData(struct queue_data *src, struct queue_data *dst)
{
    dst->fd = src->fd;
    dst->is_read = src->is_read;
    strcpy(dst->buf, src->buf);
}

void
workOnIO(struct queue_data *data)
{
    if(data->is_read)
        IORead(data);
    //else if(data->is_read == 0)
        //IOWrite(data);
}

void
IORead(struct queue_data *data)
{
    char tmp_buf[BUFFLEN];
    int i;
    int n = read(data->fd, tmp_buf, BUFFLEN);
    int request = (tmp_buf[0] - '0');
    switch(request)
    {
        case 1: //Echo back
            data->is_read = 0;
            strcpy(addHead1(data->buf, data->fd), &tmp_buf[2]);
            IOWrite(data);
            break;

        case 2: //Send to a specific user
            data->is_read = 0;
            int fd = 0;
            for(i = 2; tmp_buf[i] != '\n'; i++)
                fd = fd * 10 + (tmp_buf[i] - '0');

            strcpy(addHead2(data->buf, data->fd), &tmp_buf[++i]);
            data->fd = fd;
            IOWrite(data);
            break;

        case 3: //Broadcast
            i = 0;
            int oldfd = data->fd;
            for(; i < MAXFD; i++)
            {
                if(table[i].magic == MAGICNUM && table[i].fd != data->fd)
                {
                    strcpy(addHead2(data->buf, oldfd), &tmp_buf[2]);
                    data->fd = table[i].fd;
                    IOWrite(data);
                }
            }
            break;

        case 4: //print msg
            i = 0;
            int cnt = 0;
            for(; i < MAXFD; i++)
            {
                if(table[i].magic == MAGICNUM && table[i].fd != data->fd)
                {
                    addHead4(data->buf, table[i].fd);
                    printf("data buf is: %s", data->buf);
                    IOWrite(data);

                    //a very bad design, just for waiting
                    read(data->fd, tmp_buf, 10);
                    cnt++;
                }
            }
            strcpy(data->buf, "fin");
            data->buf[3] = 0;
            //t("Send info complete", cnt);
            IOWrite(data);
            break;

        case 5: //pressure test
            strcpy(data->buf, tmp_buf);
            IOWrite(data);
            break;
    }
}

void IOWrite(struct queue_data *data)
{
    write(data->fd, data->buf, strlen(data->buf) + 1);
}
