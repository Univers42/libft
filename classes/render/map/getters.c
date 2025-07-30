/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:16:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 20:20:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

bool    parser_has_error(t_parser_base *parser)
{
    return (parser && parser->error_state);
}

const char *parser_get_error(t_parser_base *parser)
{
    return (parser ? parser->error_message : NULL);
}

void    *parser_get_data(t_parser_base *parser)
{
    return (parser ? parser->specific_data : NULL);
}

bool    is_delimiter(char c)
{
    return (c == ' ' || c ==  '\n' || c == '\t');
}

size_t skip_delimiters(const char *data, size_t size, size_t offset)
{
    while (offset < size && is_delimiter(data[offset]))
        offset++;
    return (offset);
}

