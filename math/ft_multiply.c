/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multiply.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 19:52:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_math.h"

int ft_multiply(int x, int y)
{
    int res = 0;
    // negative is 1 if x and y have different signs, 0 otherwise
    int negative = (ft_is_negative(x) ^ ft_is_negative(y));

    // Get absolute values branchlessly
    int ax = ft_abs(x);
    int ay = ft_abs(y);

    while (ay > 0)
    {
        res += ft_add(0, ax) * (ay & 1);
        ax <<= 1;
        ay >>= 1;
    }

    res = (res ^ -negative) + negative;
    return res;
}

//#include <../stdio/ft_stdio.h>
//#include <../stdlib/ft_stdlib.h>
//
//int main(int argc, char *argv[])
//{
//    if (argc != 3)
//    {
//        ft_printf("Usage: %s <int1> <int2>\n", argv[0]);
//        return 1;
//    }
//    int x = ft_atoi(argv[1]);
//    int y = ft_atoi(argv[2]);
//    int result = ft_multiply(x, y);
//    printf("%d\n", result);
//    return 0;
//}
