/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:18:48 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:18:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	handle_ampersand(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '&'))
		return (make_token(scan, TOKEN_AMP_AMP));
	else
		return (make_token(scan, TOKEN_AMPERSAND));
}

t_token	handle_semicolon(t_scanner *scan)
{
	advance(scan);
	if (match(scan, ';'))
		return (make_token(scan, TOKEN_SEMICOLON));
	else if (match(scan, '\n'))
		return (make_token(scan, TOKEN_NEWLINE));
	return (make_token(scan, TOKEN_SEMICOLON));
}

t_token	handle_glob(t_scanner *scan)
{
	char	terminator;

	terminator = peek(scan);
	advance(scan);
	if (terminator == '?')
		return (make_token(scan, TOKEN_GLOB_QUESTION));
	if (terminator == '[')
		return (make_token(scan, TOKEN_GLOB_BRACKET_OPEN));
	else if (terminator == ']')
		return (make_token(scan, TOKEN_GLOB_BRACKET_CLOSE));
	return (error_token(scan, "Error glob token"));
}

t_token	handle_greater(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '('))
		return (make_token(scan, TOKEN_SUBST_OUT));
	if (match(scan, '>'))
	{
		return (make_token(scan, TOKEN_REDIR_APPEND));
	}
	if (match(scan, '&'))
		return (make_token(scan, TOKEN_REDIR_FD_OUT));
	if (match(scan, '|'))
		return (make_token(scan, TOKEN_REDIR_CLOBBER));
	return (make_token(scan, TOKEN_REDIR_OUT));
}
