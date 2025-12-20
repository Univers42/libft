/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:20:31 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:20:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static t_token	handle_dollar_paren(t_scanner *scan)
{
	if (peek(scan) == '(')
	{
		if (peek_next(scan) == '(')
		{
			advance(scan);
			advance(scan);
			scan->lexer_state = 1;
			return (make_token(scan, TOKEN_ARITH_EXP_START));
		}
		advance(scan);
		return (make_token(scan, TOKEN_CMD_SUBST_START));
	}
	return (make_token(scan, TOKEN_DOLLAR));
}

static t_token	handle_dollar_specials(t_scanner *scan)
{
	if (match(scan, '*'))
		return (make_token(scan, TOKEN_DOLLAR_STAR));
	if (match(scan, '#'))
		return (make_token(scan, TOKEN_DOLLAR_HASH));
	if (match(scan, '@'))
		return (make_token(scan, TOKEN_DOLLAR_AT));
	if (match(scan, '?'))
		return (make_token(scan, TOKEN_DOLLAR_QUESTION));
	if (match(scan, '$'))
		return (make_token(scan, TOKEN_DOLLAR_DOLLAR));
	if (match(scan, '!'))
		return (make_token(scan, TOKEN_DOLLAR_BANG));
	if (match(scan, '-'))
		return (make_token(scan, TOKEN_DOLLAR_DASH));
	if (match(scan, '0'))
		return (make_token(scan, TOKEN_DOLLAR_ZERO));
	if (match(scan, '{'))
		return (make_token(scan, TOKEN_PAR_EXP_START));
	return (handle_dollar_paren(scan));
}

t_token	handle_dollar(t_scanner *scan)
{
	advance(scan);
	return (handle_dollar_specials(scan));
}
