/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_empty_token_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:06:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 18:15:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdbool.h>

bool	is_empty_token_list(t_deque *tokens)
{
	if (tokens->len < 2)
		return (true);
	if (tokens->len == 2
		&& ((t_token*)deque_idx(tokens, 0))->type
		== TOKEN_NEWLINE)
		return (true);
	return (false);
}
