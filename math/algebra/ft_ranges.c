#include <sys/types.h>

ssize_t     ft_abs(ssize_t n)
{
    if (n < 0)
        return (-n);
    return (n);
}

ssize_t     ft_clamp(ssize_t n, ssize_t min, ssize_t max)
{
    if (n < min)
        return (min);
    if (n > max)
        return (max);
    return (n);
}

ssize_t   ft_smin(ssize_t a, ssize_t b)
{
    return ((a > b) * b + (a <= b) * a);
}

size_t  ft_min(size_t a, size_t b)
{
    return ((a > b) * b + (a <= b) * a);
}

size_t ft_max(size_t a, size_t b)
{
    return ((a < b) * b + (a >= b) * a);
}

size_t ft_smax(ssize_t a, ssize_t b)
{
    return ((a < b) * b + (a >= b) * a);
}
