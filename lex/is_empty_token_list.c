/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_empty_token_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 21:06:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/07 23:14:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdbool.h>

bool    is_empty_token_list(t_deque *tokens)
{
    if (tokens->len < 2)
        return (true);
    if (tokens->len == 2 && ((t_token*)deque_idx(tokens, 0))->type == TOKEN_NEWLINE)
        return (true);
    return (false);
}
