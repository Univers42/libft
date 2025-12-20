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
	size_t	needle_len;
	size_t	i;

	if (!haystack || !needle)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	i = 0;
	while (haystack[i])
	{
		if (ft_strncmp(&haystack[i], needle, needle_len) == 0)
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}

char	*ft_strnstr(const char *haystack, const char *needle, t_size n)
{
	const char	*sub_haystack;
	const char	*sub_needle;
	t_size		remaining;

	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack && n > 0)
	{
		sub_haystack = haystack;
		sub_needle = needle;
		remaining = n;
		while (*sub_haystack && *sub_needle
			&& *sub_haystack == *sub_needle && remaining > 0)
		{
			sub_haystack++;
			sub_needle++;
			remaining--;
		}
		if (*sub_needle == '\0')
			return ((char *)haystack);
		n--;
		haystack++;
	}
	return (NULL);
}
