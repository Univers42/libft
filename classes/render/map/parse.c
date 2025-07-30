/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:20:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 20:56:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "ft_stdlib.h"

bool    parse_color(t_parser_base *parser, const char *data, size_t *offset)
{
	t_ctx_data	*ctx_parser;
	uint32_t	color;
	int			hex_digits;
	char		c;

	ctx_parser = (t_ctx_data *)parser->specific_data;
	if (*offset < parser->buffer_size && data[*offset] == ',')
	{
		(*offset)++;
		color = 0;
		hex_digits = 0;
		if (data[*offset] == '0' && (*offset + 1) < parser->buffer_size &&
				(data[*offset + 1] == 'x' || data[*offset + 1] == 'X'))
				*offset += 2;
		while (*offset < parser->buffer_size && hex_digits < 6)
		{
			c = data[*offset];
			if (c >= '0' && c <= '9')
				color = (color << 4) | (c - '0');
			else if (c >= 'a' && c <= 'f')
				color = (color << 4) | (c - 'a' + 10);
			else if (c >= 'A' && c <= 'F')
				color = (color << 4) | (c - 'A' + 10);
			else
				break;
			(*offset);
			hex_digits++;
		}
		if (hex_digits == 0)
			return (false);
		ctx_parser->current_color = color;
	}
	else
		ctx_parser->current_color = 0xffffff;
	return (true);
}

bool	parse_val(t_parser_base *parser, const char *data, size_t *offset)
{
	t_ctx_data	*ctx_parser;
	int			value;
	bool		negative;
	bool		has_digits;

	ctx_parser = (t_ctx_data *)parser->specific_data;
	value = 0;
	negative = false;
	has_digits = false;
	if (*offset < parser->buffer_size && data[*offset] == '-')
	{
		negative = true;
		(*offset)++;
	}
	while (*offset < parser->buffer_size && data[*offset] >= '0' && data[*offset] <= '9')
	{
		value = value * 10 + (data[*offset] - '0');
		(*offset)++;
		has_digits = true;
	}
	if (!has_digits)
		return (false);
	if (negative)
		ctx_parser->z = -value;
	else
		ctx_parser->z = value;
	return (fdf_parse_color(parser, data, offset));
}

bool fdf_advance_line(t_parser_base *parser)
{
	t_ctx_data	*ctx_parser;

	ctx_parser = (t_ctx_data *)parser->specific_data;
    ctx_parser->x = 0;
    ctx_parser->y++;
    ctx_parser->height++;
    if (!ctx_parser->width_set)
    {
        ctx_parser->width_set = true;
        ctx_parser->width = (int)ctx_parser->values_read;
    }
    else if (ctx_parser->values_read > (size_t)ctx_parser->y * ctx_parser->width)
    {
        parser->error_state = true;
        parser->error_message = strdup("Error: Bad map - inconsistent row width");
        return (false);
    }
    else if (ctx_parser->values_read < (size_t)ctx_parser->y * ctx_parser->width)
    {
        if (!ctx_parser_realloc_data(parser))
            return (false);
        while (ctx_parser->values_read < (size_t)ctx_parser->y * ctx_parser->width)
        {
            ctx_parser->points[ctx_parser->values_read] = 0;
            ctx_parser->colors[ctx_parser->values_read] = 0xffffff;
            ctx_parser->values_read++;
        }
    }
    return (true);
}

bool fdf_parse_chunk(t_parser_base *parser, const char *data, size_t size)
{
    t_ctx_data *ctx_data;
	size_t	i;

	ctx_data = (t_ctx_data *)parser->specific_data;
    i = 0;
    while (++i < size)
    {
        i = fdf_skip_delimiters(data, size, i);
        if (i >= size)
            break ;
        if (!fdf_parse_value(parser, data, &i))
            return (false);
        if (!fdf_realloc_data(parser))
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
        skip_spaces(&i, size, data);
        if (i < size && data[i] == '\n')
        {
            if (!fdf_advance_line(parser))
                return (false);
            i++;
        }
    }
    return (true);
}

bool	fdf_finalize(t_parser *parser)
{
	t_fdf_data	*fdf;


}