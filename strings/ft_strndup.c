/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:38:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 18:56:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../render/ft_stddef.h"
# include <stdlib.h>
char	*ft_strndup(const char *s, t_size n)
{
	char	*dup;
	char	*d;

	dup = (char *)malloc(n + 1);
	if (!dup)
		return (NULL);
	d = dup;
	while (*s && n--)
		*d++ = *s++;
	*d = '\0';
	return (dup);
}