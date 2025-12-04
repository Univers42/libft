#include <stdio.h>
#include "ft_debug.h"
#include "ft_stdio.h"

// ft_assert is defined elsewhere; declare it here.
extern void ft_assert(int cond);

int main(int argc, char **argv)
{
    int cond = 0;

    if (argc > 1)
        cond = (argv[1][0] != '0'); // any arg not starting with '0' => true

    printf("Calling ft_assert with cond=%d (first: should pass, second: may fail)\n", cond);

    // Passing assertion
    ft_assert(1);

    // User-chosen assertion
    ft_assert(0);

    printf("If you see this, assertions did not abort.\n");
    return 0;
}