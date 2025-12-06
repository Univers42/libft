/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_leading.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:44:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 23:54:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_string.h"
#include "ft_ctype.h"

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
		ft_strncpy(string, start, len);
		start[len] = '\0';
	}
}

void trim_newline(char *s)
{
    if (!s)
        return;
    s[strcspn(s, "\r\n")] = '\0';
}
