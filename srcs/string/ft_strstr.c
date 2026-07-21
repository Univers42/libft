/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 12:37:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 03:04:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	if (!haystack || !needle)
		return (NULL);
	return (__builtin_strstr(haystack, needle));
}

char	*ft_strnstr(const char *haystack, const char *needle, t_size n)
{
	t_size		nlen;
	const char	*end;
	const char	*p;
	const char	*hit;

	if (!haystack || !needle)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	hit = __builtin_memchr(haystack, 0, n);
	if (hit)
		n = (t_size)(hit - haystack);
	nlen = __builtin_strlen(needle);
	end = haystack + n;
	p = haystack;
	while (p + nlen <= end)
	{
		hit = __builtin_memchr(p, needle[0], (t_size)(end - p) - nlen + 1);
		if (!hit)
			return (NULL);
		if (__builtin_strncmp(hit, needle, nlen) == 0)
			return ((char *)hit);
		p = hit + 1;
	}
	return (NULL);
}
