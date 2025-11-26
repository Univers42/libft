#include "termcap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tputs callback */
static int out_int(int c)
{
    char ch = (char)c;
    write(1, &ch, 1);
    return (c);
}

int main(void)
{
    const char *term = getenv("TERM");
    char termbuf[4096];

    if (!term)
        term = "xterm";
    /* try to load termcap once (rl_getline will try too) */
    tgetent(termbuf, term);

    printf("Mini termcap shell (using rl_getline). Commands: clear, move R C, exit\n");
    while (1)
    {
        char *line = rl_getline("\033[1;36m$ \033[0m"); /* cyan bold prompt */
        if (!line)
            break;
        if (strcmp(line, "exit") == 0)
        {
            free(line);
            break;
        }
        else if (strcmp(line, "clear") == 0)
        {
            char *cl = tgetstr("cl", NULL);
            if (cl)
                tputs(cl, 1, out_int);
            else
                write(1, "\033[H\033[2J", 7);
        }
        else
        {
            printf("You said: %s\n", line);
        }
        free(line);
    }
    puts("Bye.");
    return (0);
}
