/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprime.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:44:43 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/27 19:14:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	is_prime(int x)
{
	int i;

	i = -1;
	while(++i < x)
	{
		if (x % i == 0)
			return (0);
	}
	return (1);
}

//int main(int argc, char **argv)
//{
//	if (argc != 2)
//		return (ft_putchar('\n', STDOUT_FILENO), 1);
//	int number = ft_atoi(argv[1]);
//	is_prime(number);
//}