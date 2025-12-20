/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 18:10:21 by marvin            #+#    #+#             */
/*   Updated: 2025/12/19 18:10:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	make_token(t_scanner *scan, t_token_type type)
{
	t_token token;

	token.type = type;
	token.start = scan->start;
	token.len = (int)(scan->current - scan->start);
	token.line = scan->line;
	scan->last_token = type;
	return (token);
}
