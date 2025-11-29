/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_union_insert.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 19:37:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 00:41:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "bitmap.h"
#include "ft_memory.h"

void	inter(const char *str1, const char *str2)
{
	t_hash_seen		tab;
	unsigned char	uc;
	char			c;

	if (!str1 || !str2)
		return (NULL);
	ft_memset(&tab, 0, sizeof(tab));
	build_bitmap32(tab.seen, str2);
	while (*str1)
	{
		uc = (unsigned char)*str1;
		if (bitmap32_has(tab.seen, uc) && !bitmap32_has(tab.printed, uc))
		{
			c = (char)uc;
			write(1, &c, 1);
			tab.printed[uc >> 5] |= (1u << (uc & 31));
		}
		++str1;
	}
}

void	ft_union(const char *str1, const char *str2)
{
	uint32_t		printed[BYTE];
	char			c;
	unsigned char	uc;

	if (!str1 || !str2)
		return ;
	bitmap32_zero(printed);
	while (*str1)
	{
		uc = (unsigned char)*str1++;
		if (!bitmap32_has(printed, uc))
			(write(1, (char)&c, 1), bitmap32_set(printed, uc));
	}
	while (*str2)
	{
		uc = (unsigned char)*str2++;
		if (!bitmap32_has(printed, uc))
			(write(1, (char)&c, 1), bitmap32_set(printed, uc));
	}
}
