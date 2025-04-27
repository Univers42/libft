#include <stdio.h>
#include <stdlib.h>
#include "ft_math.h"

// Assuming ft_add is defined elsewhere
int ft_add(int x, int y);

int ft_subtract(int x, int y)
{
    return (ft_add(x, ft_add(~y, 1)));
}
//
//int main(int argc, char **argv)
//{
//    if (argc != 3)
//    {
//        printf("Usage: %s <int1> <int2>\n", argv[0]);
//        return 1;
//    }
//    int a = atoi(argv[1]);
//    int b = atoi(argv[2]);
//    int result = ft_subtract(a, b);
//    printf("%d - %d = %d\n", a, b, result);
//    return 0;
//}