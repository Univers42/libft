/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 17:56:43 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 00:45:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_memory.h"

char	*strpcpy(char *dst, const char *src)
{
	size_t	len;

	len = ft_strlen(src);
	dst[len] = 0;
	return (ft_memcpy(dst, src, len));
}
