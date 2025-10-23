# include <sys/types.h>

ssize_t ft_smod(ssize_t a, ssize_t b)
{
    if (a > 0)
        return (a % b);
    return ((b + (a % b)) % b);
}

