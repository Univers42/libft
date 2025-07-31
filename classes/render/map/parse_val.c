/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_val.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:54:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/31 23:38:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

bool parse_val(t_parser *parser, const char *data, size_t *offset)
{
    t_ctx_data *ctx_parser = parser->context;
    int value = 0;
    bool negative = false;
    bool has_digits = false;

    // Parse the numeric value
    if (*offset < parser->buffer_size && data[*offset] == '-') {
        negative = true;
        (*offset)++;
    }
    
    while (*offset < parser->buffer_size && data[*offset] >= '0' && data[*offset] <= '9') {
        value = value * 10 + (data[*offset] - '0');
        (*offset)++;
        has_digits = true;
    }
    
    if (!has_digits)
        return false;

    // Store the Z value
    ctx_parser->z = negative ? -value : value;

    // Try to parse optional color (,0xRRGGBB)
    return parse_color(parser, data, offset);
}