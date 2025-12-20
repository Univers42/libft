/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:18:51 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:18:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	handle_less(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '('))
		return (make_token(scan, TOKEN_SUBST_IN));
	if (match(scan, '<'))
	{
		if (match(scan, '-'))
			return (make_token(scan, TOKEN_REDIR_HEREDOC_STRIP));
		return (make_token(scan, TOKEN_REDIR_HEREDOC));
	}
	if (match(scan, '&'))
		return (make_token(scan, TOKEN_REDIR_FD_IN));
	if (match(scan, '>'))
		return (make_token(scan, TOKEN_REDIR_INOUT));
	return (make_token(scan, TOKEN_REDIR_IN));
}

t_token	handle_lparen(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_LEFT_PAREN));
}

t_token	handle_rparen(t_scanner *scan)
{
	advance(scan);
	if (scan->lexer_state && peek(scan) == ')')
		scan->lexer_state = 0;
	return (make_token(scan, TOKEN_RIGHT_PAREN));
}

t_token	handle_lbrace(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_LEFT_BRACE));
}

t_token	handle_rbrace(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_RIGHT_BRACE));
}
