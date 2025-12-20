/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:24:36 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:24:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_yacc.h"

bool	match_sqstring(t_dyn_str *input)
{
	const size_t	pos = input->pos;
	size_t			len;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '\'')
		return (false);
	len = 1;
	while (pos + len < input->len && dyn_str_idx(input, pos + len) != '\'')
		len++;
	if (pos + len < input->len && dyn_str_idx(input, pos + len) == '\'')
	{
		len++;
		input->pos = pos + len;
		return (true);
	}
	return (false);
}

bool	match_dqstring(t_dyn_str *input)
{
	const size_t	pos = input->pos;
	size_t			len;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '"')
		return (false);
	len = 1;
	while (pos + len < input->len && dyn_str_idx(input, pos + len) != '"')
	{
		if (dyn_str_idx(input, pos + len) == '\\' && pos + len + 1 < input->len)
			len += 2;
		else
			len++;
	}
	if (pos + len < input->len && dyn_str_idx(input, pos + len) == '"')
	{
		len++;
		input->pos = pos + len;
		return (true);
	}
	return (false);
}

bool	match_variable(t_dyn_str *input)
{
	const size_t	pos = input->pos;
	size_t			len;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '$')
		return (false);
	len = 1;
	if (pos + len < input->len
		&& (ft_isalpha(dyn_str_idx(input, pos + len))
			|| dyn_str_idx(input, pos + len) == '_'))
	{
		len++;
		while (pos + len < input->len
			&& (ft_isalnum(dyn_str_idx(input, pos + len))
				|| dyn_str_idx(input, pos + len) == '_'))
			len++;
		input->pos = pos + len;
		return (true);
	}
	return (false);
}
