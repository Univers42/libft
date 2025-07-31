/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_chunk.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:20:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/31 23:07:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "ft_stdlib.h"

bool	parse_chunk(t_parser *parser, const char *data, size_t size)
{
    t_ctx_data *ctx_data;
    size_t	i;

    ctx_data = parser->context;
    i = 0;
    while (i < size)
    {
        i = skip_delimiters(data, size, i);
        if (i >= size)
            break ;
        if (!parse_val(parser, data, &i))
            return (false);
        if (!realloc_data(parser))
            return (false);
        ctx_data->colors[ctx_data->values_read] = ctx_data->current_color;
        ctx_data->points[ctx_data->values_read] = (float)ctx_data->z;
        if (!ctx_data->z_set || ctx_data->z < ctx_data->min_z)
            ctx_data->min_z = ctx_data->z;
        if (!ctx_data->z_set || ctx_data->z > ctx_data->max_z)
            ctx_data->max_z = ctx_data->z;
        ctx_data->z_set = true;
        ctx_data->values_read++;
        ctx_data->x++;
        // Skip spaces
        while (i < size && data[i] == ' ')
            i++;
        if (i < size && data[i] == '\n')
        {
            if (!advance_line(parser))
                return (false);
            i++;
        }
    }
    return (true);
}
