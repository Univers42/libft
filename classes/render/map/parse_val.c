/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_val.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:54:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/31 23:00:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

bool	parse_val(t_parser *parser, const char *data, size_t *offset)
{
	t_ctx_data	*ctx_parser = parser->context;
	int			value;
	bool		negative;
	bool		has_digits;

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
	return (parse_color(parser, data, offset));
}