/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:03:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 18:06:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_colors.h"

const char	*get_token_name(t_token_type type)
{
	int	i;

	i = 0;
	while (tok[i].rep_name != NULL)
	{
		if (tok[i].token == type)
			return (tok[i].rep_name);
		i++;
	}
	return ("UNKNOWN");
}

/* Choose a color based on token category (very lightweight heuristic). */
const char	*token_color(t_token_type type)
{
	if (is_redir_token(type))
		return (ASCII_YELLOW);
	if (is_logical_token(type) || type == TOKEN_PIPE)
		return (ASCII_CYAN);
	if (is_keyword_token(type))
		return (ASCII_MAGENTA);
	if (type == TOKEN_WORD
		|| type == TOKEN_IDENTIFIER || type == TOKEN_NUMBER)
		return (ASCII_GREEN);
	if (type == TOKEN_ERR)
		return (ASCII_RED BOLD_TERM);
	return (ASCII_BLUE);
}

void	print_token_debug(t_token *token)
{
	const char	*name = get_token_name(token->type);
	const char	*color = token_color(token->type);

	printf("%s%-30s%s │ %3d │ ", color, name,
		RESET_TERM, token->len);
	if (token->len > 0)
		printf("%s'%.*s'%s", ASCII_GREY, token->len,
			token->start, RESET_TERM);
	else
		printf("%s(empty)%s", ASCII_GREY, RESET_TERM);
	printf("\n");
}
