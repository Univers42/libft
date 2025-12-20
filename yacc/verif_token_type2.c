/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_token_type2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:25:02 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 18:25:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	is_expansion(t_token_type type)
{
	return (
		type == TOKEN_DOLLAR
		|| type == TOKEN_DOLLAR_HASH
		|| type == TOKEN_DOLLAR_STAR
		|| type == TOKEN_DOLLAR_AT
		|| type == TOKEN_DOLLAR_QUESTION
		|| type == TOKEN_DOLLAR_DOLLAR
		|| type == TOKEN_DOLLAR_BANG
		|| type == TOKEN_DOLLAR_DASH
		|| type == TOKEN_DOLLAR_ZERO
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
