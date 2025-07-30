/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:06:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 20:15:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

t_parser_vtable *singleton(void)
{
    static  t_parser_vtable vtable = 
    {
        .parse_chunk = parse_chunk,
        .parse_value = parse_val,
        .parse_color = parse_color,
        .finalize = finalize,
        .is_delimiter = is_delimiter,
        .skip_delimiters = skip_delimiters,
        .line_advance = line_advance,
        .validate_format = validate_format,
        .cleanup = cleanup,
        .realloc_data = realloc_data,
    };

    return (&vtable);
}

void    parser_destroy(t_parser_base *parser)
{
    if (!parser)
        return ;
    if (parser->vtable && parser->vtable->cleanup)
        parser->vtable->cleanup(parser);
    free(parser->buffer);
    free(parser->error_message);
    free(parser);
}

