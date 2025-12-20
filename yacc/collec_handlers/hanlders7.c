/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hanlders7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:18:40 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:18:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_ctype.h"

static void	scan_word(t_scanner *scan, const char **start_ptr)
{
	char	c;

	*start_ptr = scan->current;
	while (!scan_is_at_end(scan))
	{
		c = peek(scan);
		if (ft_isspace((int)c) || is_metachar((int)c) || (int)c == '\n')
			break ;
		advance(scan);
	}
	if (scan->current == *start_ptr)
		advance(scan);
}

t_token	handle_word(t_scanner *scan)
{
	const char		*start_ptr;
	int				all_ident;
	int				all_digits;
	unsigned char	ch;
	const char		*p;

	scan_word(scan, &start_ptr);
	all_ident = 1;
	all_digits = 1;
	p = start_ptr;
	while (++p < scan->current)
	{
		ch = (unsigned char)*p;
		if (!(ft_isalnum(ch) || ch == '_'))
			all_ident = 0;
		if (!ft_isdigit(ch))
			all_digits = 0;
	}
	if (all_digits)
		return (make_token(scan, TOKEN_NUMBER));
	if (all_ident)
		return (make_token(scan, TOKEN_IDENTIFIER));
	return (make_token(scan, TOKEN_WORD));
}

t_token	handle_whitespace(t_scanner *scan)
{
	while (ft_isspace(peek(scan)))
		advance(scan);
	return (make_token(scan, TOKEN_WHITESPACE));
}

t_token	handle_eof(t_scanner *scan)
{
	return (make_token(scan, TOKEN_EOF));
}

t_token	handle_literal(t_scanner *scan)
{
	char	c;

	c = peek(scan);
	if (ft_isalpha(c) || c == '_')
		return (identifier(scan));
	if (ft_isdigit(c))
		return (number(scan));
	advance(scan);
	return (make_token(scan, TOKEN_WORD));
}
