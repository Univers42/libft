#include "pipe.h"
#include "ft_memory.h"

void pipe_close_pair(t_pipe_fds *p)
{
    if (!p)
        return;
    if (p->r >= 0)
    {
        close(p->r);
        p->r = -1;
    }
    if (p->w >= 0)
    {
        close(p->w);
        p->w = -1;
    }
}
