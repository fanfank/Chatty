#include "../header.h"
#include <string.h>
#define MAXSTRSIZE 10000

void IORead(struct queue_data *data);
void IOWrite(struct queue_data *data);
extern char *addHead1(char buf[], int fd);
extern char *addHead2(char buf[], int fd);
extern char *addHead3(char buf[], int fd);
extern char *addHead4(char buf[], int fd);
extern int hashFind(int);
extern struct hashtable table[];
extern struct list htlist;
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
    if(n <= 0)
    {
        t("Unknow message, n is:", n);
        return;
    }

    struct hashtable *l;
    int request = (tmp_buf[0] - '0');
    int key;
    switch(request)
    {
        case 0: //ready
            key = hashFind(data->fd);
            if(key >= 0)
            {
                table[key].ready = 1;
            }
            else
            {
                t("Can't set ready, no key of fd",data->fd);
            }
            break;

        case 1: //Echo back
            data->is_read = 0;
            strcpy(addHead1(data->buf, data->fd), &tmp_buf[2]);
            data->buf[BUFFLEN - 1] = 0;
            IOWrite(data);
            break;

        case 2: //Send to a specific user
            data->is_read = 0;
            int fd = 0;
            for(i = 2; tmp_buf[i] != '\n'; i++)
                fd = fd * 10 + (tmp_buf[i] - '0');

            t("#1 Sent to fd:", fd);
            key = hashFind(fd);
            t("#2 Sent to fd:", fd);
            if(key < 0)
            {
                strcpy(data->buf, "The client specified no longer exists");
                IOWrite(data);
                break;
            }
            else if(table[key].ready != 1)
            {
                strcpy(data->buf, "The client specified is not ready");
                IOWrite(data);
                break;
            }

            t("#3 Sent to fd:", fd);
            strcpy(addHead2(data->buf, data->fd), &tmp_buf[++i]);
            t("#4 Sent to fd:", fd);
            data->fd = fd;
            data->buf[BUFFLEN - 1] = 0;
            IOWrite(data);
            break;

        case 3: //Broadcast
            i = 0;
            data->is_read = 0;
            int oldfd = data->fd;
            Pthread_rwlock_rdlock(&htlist.htlist_rwlock);
            l = htlist.ht_head.next;
            while(l->magic == MAGICNUM)
            {
                if(l->fd != oldfd && l->ready == 1)
                {
                    strcpy(addHead3(data->buf, oldfd), &tmp_buf[2]);
                    data->fd = l->fd;
                    data->buf[BUFFLEN - 1] = 0;
                    IOWrite(data);
                }
                l = l->next;
            }
            Pthread_rwlock_unlock(&htlist.htlist_rwlock);
            break;

        case 4: //print addresses
            key = hashFind(data->fd);
            if(key < 0)
                break;
            table[key].ready = -1;
            int cnt = 0;

            l = htlist.ht_head.next;
            Pthread_rwlock_rdlock(&htlist.htlist_rwlock);
            while(l->magic == MAGICNUM)
            {
                if(l->fd != data->fd)
                {
                    addHead4(data->buf, l->fd);
                    data->buf[BUFFLEN - 1] = 0;
                    //t(data->buf, 1);
                    IOWrite(data);

                    //a very bad design, just for waiting
                    read(data->fd, tmp_buf, 10);
                    cnt++;
                }
                l = l->next;
            }
            Pthread_rwlock_unlock(&htlist.htlist_rwlock);

            strcpy(data->buf, "fin");
            data->buf[3] = 0;
            t("Send info complete", cnt);
            IOWrite(data);
            break;

        case 5: //pressure test
            strcpy(data->buf, tmp_buf);
            IOWrite(data);
            break;

        case 9:
            key = hashFind(data->fd);
            if(key >= 0)
            {
                table[key].ready = 0;
            }
            else
            {
                t("Can't set !ready, no key of fd",data->fd);
            }
            break;

        default:
            t("Unknown protocol", -1);
            break;
    }
}

void IOWrite(struct queue_data *data)
{
    //t(data->buf, 0);
    write(data->fd, data->buf, strlen(data->buf) + 1);
}
