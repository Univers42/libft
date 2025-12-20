/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:25:16 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 22:25:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_yacc.h"

bool	match_comment(t_dyn_str *input)
{
	const size_t	pos = input->pos;
	size_t			len;

	ft_assert(input && input->buff);
	if (pos >= input->len || dyn_str_idx(input, pos) != '#')
		return (false);
	len = 1;
	while (pos + len < input->len && dyn_str_idx(input, pos + len) != '\n')
		len++;
	input->pos = pos + len;
	return (true);
}

bool	match_identifier(t_dyn_str *input)
{
	const size_t	pos = input->pos;
	size_t			len;

	ft_assert(input && input->buff);
	if (pos >= input->len)
		return (false);
	if (!ft_isalpha(dyn_str_idx(input, pos)) && dyn_str_idx(input, pos) != '_')
		return (false);
	len = 1;
	while (pos + len < input->len
		&& (ft_isalnum(dyn_str_idx(input, pos + len))
			|| dyn_str_idx(input, pos + len) == '_'))
		len++;
	input->pos = pos + len;
	return (true);
}

bool	match_number(t_dyn_str *input)
{
	const size_t	pos = input->pos;
	size_t			len;

	ft_assert(input && input->buff);
	if (pos >= input->len || !ft_isdigit(dyn_str_idx(input, pos)))
		return (false);
	len = 0;
	while (pos + len < input->len && ft_isdigit(dyn_str_idx(input, pos + len)))
		len++;
	if (pos + len < input->len && dyn_str_idx(input, pos + len) == '.')
	{
		len++;
		while (pos + len < input->len
			&& ft_isdigit(dyn_str_idx(input, pos + len)))
			len++;
	}
	input->pos = pos + len;
	return (true);
}
