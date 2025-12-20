/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:18:44 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:18:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	handle_percent(t_scanner *scan)
{
	advance(scan);
	if (scan->lexer_state)
	{
		if (match(scan, '='))
			return (make_token(scan, TOKEN_ASS_MOD));
		return (make_token(scan, TOKEN_OP_MOD));
	}
	if (match(scan, '%'))
		return (make_token(scan, TOKEN_PERCENT_PERCENT));
	return (make_token(scan, TOKEN_PERCENT));
}

t_token	handle_eq(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '='))
		return (make_token(scan, TOKEN_COMP_EQ));
	else if (match(scan, '+'))
		return (make_token(scan, TOKEN_ASS_PLUS));
	return (make_token(scan, TOKEN_ASS_EQUAL));
}

t_token	handle_colon(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '-'))
		return (make_token(scan, TOKEN_COLON_DASH));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_COLON_EQUAL));
	else if (match(scan, '+'))
		return (make_token(scan, TOKEN_COLON_PLUS));
	else if (match(scan, '?'))
		return (make_token(scan, TOKEN_COLON_QUESTION));
	return (make_token(scan, TOKEN_WORD));
}

t_token	handle_question(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '-'))
		return (make_token(scan, TOKEN_COLON_DASH));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_COLON_EQUAL));
	else if (match(scan, '+'))
		return (make_token(scan, TOKEN_COLON_QUESTION));
	return (make_token(scan, TOKEN_ERR));
}

t_token	handle_bang(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '!'))
		return (make_token(scan, TOKEN_BANG_BANG));
	return (make_token(scan, TOKEN_BANG));
}
