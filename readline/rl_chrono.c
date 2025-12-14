#include "ft_time.h"
#include <sys/time.h>

/*
** Provide single canonical chrono storage and helpers used by prompt code.
** This avoids multiple-definition/inline issues and supplies the linker symbol.
*/

t_chrono *get_chrono(void)
{
    /* zero-initialize fields for safety */
    static t_chrono g = {{0, 0}, 0};
    return (&g);
}

void prompt_start_timer(void)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == 0)
        get_chrono()->start = tv;
}

void prompt_stop_timer(void)
{
    struct timeval end;
    if (gettimeofday(&end, NULL) == 0)
    {
        t_chrono *c = get_chrono();
        c->last_ms = (end.tv_sec - c->start.tv_sec) * 1000L + (end.tv_usec - c->start.tv_usec) / 1000L;
    }
}
