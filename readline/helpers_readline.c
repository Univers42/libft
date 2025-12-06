#include "readline.h"
#include <stdlib.h>
#include "ft_stdio.h"

char *ft_readline(const char *prompt)
{
    char pbuf[256];

    if (!prompt || !*prompt)
        ft_snprintf(pbuf, sizeof(pbuf), "%s", "❯ ");
    else
        ft_snprintf(pbuf, sizeof(pbuf), "%s", prompt);
    return rl_getline(pbuf);
}
