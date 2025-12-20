/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:18:46 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:18:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	handle_backslash(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_BACKSLASH));
}

t_token	handle_hash(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '#'))
		return (make_token(scan, TOKEN_POUND_POUND));
	return (make_token(scan, TOKEN_POUND));
}

t_token	handle_tilde(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_TILDE));
}

t_token	handle_star(t_scanner *scan)
{
	advance(scan);
	if (scan->lexer_state)
	{
		if (match(scan, '='))
			return (make_token(scan, TOKEN_ASS_MULT));
		return (make_token(scan, TOKEN_OP_MULT));
	}
	if (match(scan, '*'))
		return (make_token(scan, TOKEN_GLOB_STAR));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_ASS_MULT));
	return (make_token(scan, TOKEN_GLOB_STAR));
}

t_token	handle_plus(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '+'))
		return (make_token(scan, TOKEN_OP_INCREMENT));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_ASS_PLUS));
	return (make_token(scan, TOKEN_OP_ADD));
}
