/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lcm.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 19:30:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_math.h"
#include <stdio.h>

int ft_lcm(int x, int y)
{
    int hcf = ft_hcf(x, y);
    if (hcf == 0)
        return 0;
    return (x / hcf) * y;
}

//int main()
//{
//    int a = 12;
//    int b = 18;
//    int result = ft_lcm(a, b);
//
//    printf("LCM of %d and %d is %d\n", a, b, result);
//    return 0;
//}
