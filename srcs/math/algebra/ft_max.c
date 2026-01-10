/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_max.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/22 01:06:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algebra.h"
#include <stdlib.h>

int	ft_max(int a, int b)
{
	int	diff;
	int	mask;

	diff = ft_subtract(a, b);
	mask = diff >> (sizeof(int) * 8 - 1);
	return ((a & ~mask) | (b & mask));
}

size_t	ft_max2(size_t a, size_t b)
{
	return ((a < b) * b + (a >= b) * a);
}

size_t max_size(size_t a, size_t b)
{
	if (a > b)
		return (a);
	return (b);
}

//int main(int argc, char **argv)
//{
//    if (argc != 3)
//        return 1;
//    int a = atoi(argv[1]);
//    int b = atoi(argv[2]);
//   	ft_printf("max(%d, %d) = %d\n", a, b, ft_max(a, b));
//    return 0;
//}
