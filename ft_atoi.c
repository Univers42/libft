/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 12:23:59 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/26 19:42:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "./TDD/test.h"
#include "limits.h"

static int	ft_isspace(int c)
{
	return (c && (c == ' ' || c == '\t'
			|| c == '\n' || c == '\r'
			|| c == '\v' || c == '\f'));
}

/**
 * Convert a nptring to an integer
 * @param nptr The input nptring
 * @return The converted integer
 * @note This function skips leading whitespaces, 
 * handles an optional sign, and converts the numerical 
 * characters to an integer.
 */
int	ft_atoi(const char *str)
{
	long	result;
	int		sign;
	int		digit;

	result = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		digit = *str - '0';
		if (sign == 1 && result > (LONG_MAX - digit) / 10)
			return (INT_MAX);
		if (sign == -1 && (-result) < (LONG_MIN + digit) / 10)
			return (INT_MIN);
		result = (result * 10) + digit;
		str++;
	}
	return ((int)(result * sign));
}
