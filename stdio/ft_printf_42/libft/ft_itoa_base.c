/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:16:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/03/24 23:15:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_get_digits(t_size n, int base, char *str, int *i)
{
	int	mod;

	while (n)
	{
		mod = n % base;
		if (mod > 9)
			str[(*i)++] = mod - 10 + 'a';
		else
			str[(*i)++] = mod + '0';
		n /= base;
	}
	str[*i] = '\0';
}

char	*ft_itoa_base(t_size n, int base)
{
	char	*str;
	int		i;

	if (base < 2 || base > 16)
		return (NULL);
	str = (char *)malloc(65);
	if (!str)
		return (NULL);
	i = 0;
	if (n == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return (str);
	}
	ft_get_digits(n, base, str, &i);
	return (ft_strrev(str));
}
