/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_modulo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 10:00:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_math.h"

int ft_modulo(int a, int b)
{
    if (b == 0)
        return 0;
    int sign = (a < 0) ? -1 : 1;
    unsigned int ua = (a < 0) ? -a : a;
    unsigned int ub = (b < 0) ? -b : b;
    for (int i = 31; i >= 0; i--)
    {
        if ((ua >> i) >= ub)
            ua = ft_subtract(ua, (ub << i));
    }
    return (sign * ua);
}

//int main(int argc, char **argv)
//{
//    if (argc != 3)
//        return 1;
//    int a = atoi(argv[1]);
//    int b = atoi(argv[2]);
//    printf("%d %% %d = %d\n", a, b, ft_modulo(a, b));
//    return 0;
//}