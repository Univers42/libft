/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_super_trim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 22:38:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 20:29:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "trim.h"

static int	should_skip(char c, int flags);
static int	parse_sign(t_addr *ptr);
static int	parse_int(t_addr *ptr);

void	ft_super_trim(t_addr *ptr, int flags)
{
	while (*(char *)*ptr && (should_skip(*(char *)*ptr, flags) & ST_OK))
		*ptr = (char *)*ptr + 1;
}

static int	should_skip(char c, int flags)
{
	if ((flags & TRIM_SPACES)
		&& (c == ' ' || c == '\t' || c == '\n' || c == '\r'))
		return (ST_OK);
	if ((flags & TRIM_NUMS) && (c >= '0' && c <= '9'))
		return (ST_OK);
	if ((flags & TRIM_RADIX)
		&& ((c >= '0' && c <= '9')
			|| (c >= 'a' && c <= 'f')
			|| (c >= 'A' && c <= 'F')))
		return (ST_OK);
	return (ST_ERR_BASE);
}

static int	parse_sign(t_addr *ptr)
{
	int	sign;

	sign = 1;
	if (*(char *)*ptr == '-')
	{
		sign = -1;
		*ptr = (char *)*ptr + 1;
	}
	else if (*(char *)*ptr == '+')
		*ptr = (char *)*ptr + 1;
	return (sign);
}

static int	parse_int(t_addr *ptr)
{
	int	val;

	val = 0;
	while (*(char *)*ptr >= '0' && *(char *)*ptr <= '9')
	{
		val = val * 10 + (*(char *)*ptr - '0');
		*ptr = (char *)*ptr + 1;
	}
	return (val);
}
