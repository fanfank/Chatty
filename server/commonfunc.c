#include "header.h"
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
