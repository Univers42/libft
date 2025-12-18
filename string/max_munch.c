/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   max_munch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 22:29:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/17 03:06:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include <string.h>
#include <stdint.h>

int	max_munch(char *haystack, void *needles, size_t offset, size_t size)
{
	int		max_idx;
	int		max_len;
	int		i;
	char	*pattern;
	int		pattern_len;

	max_idx = -1;
	max_len = -1;
	i = 0;
	while (1)
	{
		uintptr_t	addr = (uintptr_t)needles + (i * size) + offset;
		/* copy pointer from possibly unaligned memory */
		memcpy(&pattern, (const void *)addr, sizeof(pattern));
		if (!pattern)
			break;
		pattern_len = ft_strlen(pattern);
		if (pattern_len > max_len
			&& ft_strncmp(pattern, haystack, pattern_len) == 0)
		{
			max_idx = i;
			max_len = pattern_len;
		}
		i++;
	}
	return (max_idx);
}
