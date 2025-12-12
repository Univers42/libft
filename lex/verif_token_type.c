/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_token_type.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:13:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 03:00:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdbool.h>

bool	is_redir_token(t_token_type type)
{
	return (type >= TOKEN_REDIR_IN && type <= TOKEN_REDIR_CLOBBER);
}

bool	is_logical_token(t_token_type type)
{
	return (type == TOKEN_PIPE_PIPE || type == TOKEN_AMP_AMP);
}

bool	is_keyword_token(t_token_type type)
{
	return (type >= TOKEN_IF && type <= TOKEN_SELECT);
}

bool	is_quote_token(t_token_type	type)
{
	return (type >= TOKEN_SQUOTE_START && type <= TOKEN_BQUOTE_END);
}

bool	is_expansion(t_token_type type)
{
	return (
		type == TOKEN_DOLLAR ||
		type == TOKEN_DOLLAR_HASH ||
		type == TOKEN_DOLLAR_STAR ||
		type == TOKEN_DOLLAR_AT ||
		type == TOKEN_DOLLAR_QUESTION ||
		type == TOKEN_DOLLAR_DOLLAR ||
		type == TOKEN_DOLLAR_BANG ||
		type == TOKEN_DOLLAR_DASH ||
		type == TOKEN_DOLLAR_ZERO
	);
}

bool	is_simple_cmd_token(t_token_type tt)
{
	return (tt == TOKEN_REDIR_FD_IN
		|| tt == TOKEN_WORD
		|| tt == TOKEN_REDIR_FD_OUT
		|| tt == TOKEN_REDIR_APPEND
		|| tt == TOKEN_REDIR_HEREDOC);
}

bool	is_simple_list_op(t_token_type tt)
{
	return (tt == TOKEN_SEMICOLON
		|| tt == TOKEN_LOGICAL_OR
		|| tt == TOKEN_LOGICAL_AND);
}

bool	is_compund_list_op(t_token_type tt)
{
	return (is_simple_list_op(tt) || tt == TOKEN_NEWLINE);
}

bool	is_redirect(t_token_type tt)
{
	return (tt == TOKEN_REDIR_FD_IN
		|| tt == TOKEN_REDIR_FD_OUT
		|| tt == TOKEN_REDIR_APPEND
		|| tt == TOKEN_REDIR_HEREDOC);
}