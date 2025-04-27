/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_power_of_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:18:49 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 19:20:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../stdio/ft_stdio.h"
#include "../stdlib/ft_stdlib.h"

int ft_is_power2(int x)
{
    return (x > 0 && (x & (x - 1)));
}

//int main(int argc, char **argv)
//{
//    if (argc != 2)
//    {
//        ft_printf("Usage: %s <number>\n", argv[0]);
//        return 1;
//    }
//
//    int num = ft_atoi(argv[1]);
//    if (!ft_is_power2(num))
//        ft_printf("%d is a power of 2.\n", num);
//    else
//        ft_printf("%d is not a power of 2.\n", num);
//
//    return 0;
//}