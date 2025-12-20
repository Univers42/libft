/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:08:25 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 18:08:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	skip_unused_data(t_scanner *scan)
{
	char	c;

	while (ST_SCANNING)
	{
		c = peek(scan);
		if (ft_isspace(c))
			advance(scan);
		if (assert_char(c, '\n'))
		{
			scan->line++;
			advance(scan);
		}
		if (assert_char(c, '/'))
		{
			if (peek_next(scan) == '/')
				while (peek(scan) != '\n' && !scan_is_at_end(scan))
					advance(scan);
			else
				return ;
		}
		else
			break ;
	}
	return ;
}
