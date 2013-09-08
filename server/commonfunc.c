#include "header.h"
int
clearStr(char buf[], int len)
{
    if(buf == 0)
        return -1;
    memset(buf, 0, sizeof(char)*len);
    return 0;
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

void t(char s[], int i){printf("%s %d\n", s, i);fflush(stdout);}
