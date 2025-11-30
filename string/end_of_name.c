/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_of_name.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 17:58:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 18:01:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_ctype.h"

char	*end_of_name(const char *name)
{
	char	*p;

	p = (char *)name;
	if (!is_name_char(*p))
		return (p);
	while (*++p)
		if (! is_in_name(*p))
			break ;
	return (p);
}
