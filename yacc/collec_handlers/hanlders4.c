/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hanlders4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:18:42 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:18:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	handle_pipe(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '|'))
		return (make_token(scan, TOKEN_PIPE_PIPE));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_ASS_OR));
	else
		return (make_token(scan, TOKEN_PIPE));
}

t_token	handle_div(t_scanner *scan)
{
	if (scan->lexer_state)
	{
		advance(scan);
		if (match(scan, '='))
			return (make_token(scan, TOKEN_ASS_DIV));
		return (make_token(scan, TOKEN_OP_DIV));
	}
	return (handle_word(scan));
}

t_token	handle_lbracket(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_GLOB_BRACKET_OPEN));
}

t_token	handle_rbracket(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_GLOB_BRACKET_CLOSE));
}

t_token	handle_dot(t_scanner *scan)
{
	advance(scan);
	if (ft_isdigit(peek(scan)))
	{
		while (ft_isdigit(peek(scan)))
			advance(scan);
		return (make_token(scan, TOKEN_NUMBER));
	}
	if ((scan->last_token == TOKEN_GLOB_STAR
			|| scan->last_token == TOKEN_GLOB_QUESTION)
		&& (ft_isalpha(peek(scan)) || peek(scan) == '_'))
	{
		scan->start = scan->current;
		return (identifier(scan));
	}
	return (make_token(scan, TOKEN_DOT));
}
