/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_leading.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:44:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 16:49:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"

/**
 * Remove all leading whitespace from STRING. This includes
 * newlines. STRING should be terminated with zero
 */
void	strip_leading(char *string)
{
	char	*start;
	size_t	len;

	start = string;
	while (*string && (ft_isspace(*string) || *string == '\n'))
		string++;
	if (string != start)
	{
		len = ft_strlen(string);
		fast_copy(string, start, len);
		start[len] = '\0';
	}
}
