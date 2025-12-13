#include "pipe.h"

/* set or clear O_NONBLOCK */
int set_nonblock(int fd, int enable)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        return (-1);
    if (enable)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == 0)
        return (0);
    return (-1);
}