/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strblock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 19:49:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 19:51:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void    ft_str_clear(char *s)
{
    while (s && *s)
        *s = '\0';
}

void    ft_strdel(char **da)
{
    if (da)
    {
        free(*da);
        *da = NULL;
    }
}

int	ft_strequ(char const *s1, char const *s2)
{
	if (!s1 && !s2)
		return (1);
	if (s1 && s2)
	{
		while (*s1 != '\0' || *s2 != '\0')
		{
			if (*s1 != *s2)
				return (0);
			s1++;
			s2++;
		}
	}
	return (1);
}

int	ft_strnequ(char const *s1, char const *s2, size_t n)
{
	unsigned int	i;

	i = n;
	if ((!s1 && !s2) || !n)
		return (1);
	if (s1 && s2 && n)
	{
		while (i != 0)
		{
			if (*s1 != *s2)
				return (0);
			s1++;
			s2++;
			i--;
		}
	}
	return (1);
}

char	*ft_strnew(size_t size)
{
	char	*arr;

	if (size >= 9223372036854775807)
		return (NULL);
	arr = (char*)malloc(sizeof(char) * (size + 1));
	if (!arr)
		return (NULL);
	arr[size] = '\0';
	while (size--)
		arr[size] = '\0';
	return (arr);
}
