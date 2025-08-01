/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:04:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/31 23:04:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

bool store_parsed_color(t_parser *parser, t_token *token)
{
    t_ctx_data *ctx = parser->context;
    
    if (ctx->colors && ctx->values_read > 0) {
        ctx->colors[ctx->values_read - 1] = token->data.color_val;
    }
    
    return true;
}
