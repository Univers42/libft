/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_simple_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 20:26:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 20:28:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

bool    is_export(t_ast_node word)
{
    t_ast_node c;

    if (word.children.len != 1)
        return (false);
    c = word.children.buff[0];
    if (c.token.tt != TOKEN_WORD)
        return (false);
    if (ft_strncmp(c.token.start, "export", c.token.len))
        return (false);
    return (true);
}

int expand_simple_cmd_assignment(t_expander_simple_cmd *exp, t_executable_cmd *ret)