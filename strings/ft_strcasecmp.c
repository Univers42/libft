/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcasecmp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 02:36:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/08 02:40:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_ctype.h"

int my_strcasecmp(const char *s1, const char *s2)
{
	unsigned char   c1;
	unsigned char   c2;

	while (*s1++ && *s2++)
	{
		c1 = (unsigned char)tolower((unsigned char)*s1);
		c2 = (unsigned char)tolower((unsigned char)*s2);
		if (c1 != c2)
			return (c1 - c2);
		s1++;
		s2++;
	}
	return (unsigned char)tolower(*s1) - (unsigned char)tolower(*s2);
}
