/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substring.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:57:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 21:04:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include <stdlib.h>

char	*substring(const char *string, size_t start, size_t end)
{
	size_t	len;
	char	*result;

	len = end - start;
	result = (char *)malloc(len + 1);
	if (!result)
		return ((char *) NULL);
	ft_strncpy(result, string + start, len);
	return (result);
}
