#ifndef COMMONFUNC_C
#define COMMONFUNC_C
#include "header.h"
void
checkInRange(int *num, int range_bg, int range_ed)
{
    int i;
    char tmp[BUFFLEN];
    int first_time = 1;
    *num = range_ed + 1;
    while(*num < range_bg || *num > range_ed)
    {
        if(first_time == 1)
            first_time = 0;
        else
            printf("Unknown command\n");
        scanf("%s", tmp);
        i = *num = 0;
        for(i = 0; tmp[i]; i++)
            if(tmp[i] < '0' || tmp[i] > '9')
            {
                *num = -1;
                break;
            }
            else
            {
                *num = *num * 10 + (tmp[i] - '0');
            }
    }
}

void 
clrBuf()
{
    while(getc(stdin) != '\n');
}

void
_err_quit(const char *s, int exit_code)
{
    printf("%s: %d\n", s, exit_code);
    exit(exit_code);
}
#endif
