/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:49:34 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 00:49:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_yacc.h"

bool	match_operator(t_dyn_str *input)
{
	const char			*operators[] = {
		"==", "!=", "<=", ">=", "++", "--", "+=", "-=", NULL
	};
	const size_t		pos = input->pos;
	size_t				i;
	size_t				len;

	ft_assert(input && input->buff);
	if (pos >= input->len)
		return (false);

	/* handle parameter-expansion colon-operators first (:-, :=, :?, :+) */
	if (pos + 1 < input->len && dyn_str_idx(input, pos) == ':')
	{
		char a = dyn_str_idx(input, pos + 1);
		if (a == '-')
			return (dyn_str_update_pos(input, pos + 2), true);
		if (a == '=')
			return (dyn_str_update_pos(input, pos + 2), true);
		if (a == '?')
			return (dyn_str_update_pos(input, pos + 2), true);
		if (a == '+')
			return (dyn_str_update_pos(input, pos + 2), true);
	}

	i = -1;
	while (operators[++i])
	{
		len = ft_strlen(operators[i]);
		if (pos + len <= input->len
			&& ft_strncmp(input->buff + pos, operators[i], len) == 0)
			return (dyn_str_update_pos(input, pos + len), true);
	}
	if (ft_strchr("+-*/<>=!&|", dyn_str_idx(input, pos)))
		return (dyn_str_update_pos(input, pos + 1), true);
	return (false);
}
