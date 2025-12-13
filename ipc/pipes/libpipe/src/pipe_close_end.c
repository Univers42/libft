#include "pipe.h"
#include "ft_memory.h"

void pipe_close_end(t_pipe_fds *p, int end)
{
    if (!p)
        return;
    if (end == 0 && p->r >= 0)
    {
        close(p->r);
        p->r = -1;
    }
    else if (end == 1 && p->w >= 0)
    {
        close(p->w);
        p->w = -1;
    }
}