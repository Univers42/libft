/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:20:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/31 23:29:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "ft_stdlib.h"

bool parse_token_sequence(t_parser *parser);

bool    parse_color(t_parser *parser, const char *data, size_t *offset)
{
	t_ctx_data	*ctx_parser;
	uint32_t	color;
	int			hex_digits;
	char		c;

	ctx_parser = parser->context;
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
			(*offset)++;
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

bool parse_main_loop(t_parser *parser)
{
    parser->position = 0;
    
    while (parser->position < parser->buffer_size) {
        // Skip whitespace and delimiters at start
        parser->position = parser->method->skip_delimiter(parser, parser->position);
        
        if (parser->position >= parser->buffer_size)
            break;
        
        // Handle newlines
        if (parser->buffer[parser->position] == '\n') {
            if (!parser->method->advance_line(parser))
                return false;
            parser->position++;
            continue;
        }
        
        // Parse tokens based on configuration
        if (!parse_token_sequence(parser))
            return false;
    }
    
    return parser->method->finalize(parser);
}