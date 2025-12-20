/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advance.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:42:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 18:27:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	advance_dquote(char **str)
{
	bool	prev_bs;

	ft_assert(**str == '\"');
	(*str)++;
	prev_bs = false;
	while (**str && (**str != '\"' || prev_bs))
	{
		prev_bs = **str == '\\' && !prev_bs;
		(*str)++;
	}
	if (**str != '\"')
		return (1);
	(*str)++;
	return (0);
}

int	advance_squote(char **str)
{
	ft_assert(**str == '\'');
	(*str)++;
	while (**str && **str != '\'')
		(*str)++;
	if (**str != '\'')
		return (1);
	(*str)++;
	return (0);
}

void	advance_bs(char **str)
{
	ft_assert(**str == '\\');
	if ((*str)[1])
		*str += 1;
	*str += 1;
}

bool	scan_is_at_end(t_scanner *scan)
{
	return (*scan->current == '\0');
}

char	advance(t_scanner *scan)
{
	scan->current++;
	return (scan->current[-1]);
}
