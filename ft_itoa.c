/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:11:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/26 21:26:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len(int n)
{
	size_t	count;

	count = 0;
	if (n <= 0)
		++count;
	while (n)
	{
		++count;
		n /= 10;
	}
	return (count);
}

/**
 * Converts an integer to a null-terminated string using decimal representation.
 * 
 * @param n The integer to convert.
 * @return A pointer to the newly allocated string representing the integer, 
 *         or NULL if memory allocation fails.
 * 
 * @note The returned string must be freed by the caller to avoid memory leaks.
 */
char	*ft_itoa(int n)
{
	char	*ptr;
	int		len;

	len = ft_len(n);
	ptr = malloc(len + 1);
	if (NULL == ptr)
		return (NULL);
	*(ptr + len) = '\0';
	if (n == 0)
		*ptr = '0';
	else if (n < 0)
		*ptr = '-';
	while (n)
	{
		if (n < 0)
			*(ptr + --len) = (~(n % 10) + 1) + 48;
		else
			*(ptr + --len) = (n % 10) + 48;
		n /= 10;
	}
	return (ptr);
}
