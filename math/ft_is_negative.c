/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnegative.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:32:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 19:36:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../stdio/ft_stdio.h"
#include "../stdlib/ft_stdlib.h"

int ft_is_negative(int x)
{
    int mask;

    mask = x >> sizeof(int) * 8 - 1;
    return mask & 1;
}

//int main(int argc, char **argv)
//{
//    if (argc != 2)
//    {
//        ft_printf("Usage: %s <integer>\n", argv[0]);
//        return 1;
//    }
//    int x = ft_atoi(argv[1]);
//    if (ft_is_negative(x))
//        ft_printf("%d is negative\n", x);
//    else
//        ft_printf("%d is not negative\n", x);
//    return 0;
//}
//