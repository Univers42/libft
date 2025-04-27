/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tab_mult.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 20:00:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_math.h"

int ft_tab_mult(int x)
{
    int i = 0;
    while (++i < 10)
        ft_printf("%d x %d = %d\n", x, i, ft_multiply(x, i));
    return 0;
}

//int main(void)
//{
//    int x;
//
//    ft_printf("Enter a number : ");
//    if (scanf("%d", &x) != 1)
//    {
//        ft_printf("Invalid input.\n");
//        return 1;
//    }
//    return 0;
//}