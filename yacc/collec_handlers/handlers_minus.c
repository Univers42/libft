/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_minus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:20:08 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:20:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_ctype.h"

static t_token	handle_minus_flags(t_scanner *scan)
{
	char	c;

	while (!scan_is_at_end(scan))
	{
		c = peek(scan);
		if (ft_isspace(c) || is_metachar(c))
			break ;
		advance(scan);
	}
	return (make_token(scan, TOKEN_FLAG));
}

t_token	handle_minus(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '-'))
		return (handle_minus_flags(scan));
	if (ft_isspace(peek(scan)) || is_metachar(peek(scan))
		|| scan_is_at_end(scan))
		return (make_token(scan, TOKEN_OP_MINUS));
	return (handle_minus_flags(scan));
}
