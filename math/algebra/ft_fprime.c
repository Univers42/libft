/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:02:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/04 14:40:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "algebra.h"
#include <stdio.h>

void	ft_fprime(int n)
{
	int	i;
	int	first;

	i = 2;
	first = 1;
	while (i * i <= n)
	{
		while (!(n % i))
		{
			if (!first)
				printf("*");
			printf("%d", i);
			n /= i;
			first = 0;
		}
		i++;
	}
	if (n > 1)
	{
		if (!first)
			printf("*");
		printf("%d", n);
	}
	printf("\n");
}

//int main(int argc, char **argv)
//{
//    if (argc != 2)
//        return 1;
//    int n = atoi(argv[1]);
//    ft_fprime(n);
//    return 0;
//}
//