/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advance.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:42:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 21:49:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int advance_dquote(char **str)
{
    bool    prev_bs;

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

int advance_squote(char **str)
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

void    advance_bs(char **str)
{
    ft_assert(**str == '\\');
    if ((*str)[1])
        *str += 1;
    *str += 1;
}
