/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:35:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/31 23:13:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

// Remove all references to t_handler_fn, jump_table, TOKEN_* macros, and FSM_* enums.
// Use only the new parser structure and method table as defined in map.h.

bool parse_with_fsm(t_parser *parser)
{
    // This function is now obsolete with the new design.
    // You should use parser_parse() and the method table instead.
    // If you want to keep a stub for legacy, just return false or true.
    (void)parser;
    return false;
}

// Remove preset_token_flags (no token_flags in struct)
#if 0
void preset_token_flags(t_parser *parser, uint32_t flags)
{
    if (parser)
        parser->token_flags = flags;
}
#endif

bool parse_numeric_value(t_parser *parser)
{
    t_ctx_data *ctx = parser->context;
    size_t pos = parser->position;
    int value = 0;
    bool negative = false;
    bool has_digits = false;

    if (pos < parser->buffer_size && parser->buffer[pos] == '-')
    {
        negative = true;
        pos++;
    }
    else if (pos < parser->buffer_size && parser->buffer[pos] == '+')
    {
        pos++;
    }

    while (pos < parser->buffer_size && parser->buffer[pos] >= '0' && parser->buffer[pos] <= '9')
    {
        value = value * 10 + (parser->buffer[pos] - '0');
        pos++;
        has_digits = true;
    }

    if (!has_digits)
        return false;

    ctx->z = negative ? -value : value;
    parser->position = pos;
    return true;
}

bool parse_color_value(t_parser *parser)
{
    t_ctx_data *ctx = parser->context;
    size_t pos = parser->position;

    if (pos >= parser->buffer_size || parser->buffer[pos] != ',')
    {
        ctx->current_color = 0xffffff;  // Default color
        return true;
    }

    pos++;  // Skip comma
    uint32_t color = 0;
    int hex_digits = 0;

    // Skip 0x prefix if present
    if (pos + 1 < parser->buffer_size &&
        parser->buffer[pos] == '0' &&
        (parser->buffer[pos + 1] == 'x' || parser->buffer[pos + 1] == 'X'))
    {
        pos += 2;
    }

    // Parse hex digits
    while (pos < parser->buffer_size && hex_digits < 6)
    {
        char c = parser->buffer[pos];
        if (c >= '0' && c <= '9')
            color = (color << 4) | (c - '0');
        else if (c >= 'a' && c <= 'f')
            color = (color << 4) | (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            color = (color << 4) | (c - 'A' + 10);
        else
            break;
        pos++;
        hex_digits++;
    }

    if (hex_digits == 0)
        return false;

    ctx->current_color = color;

    // Store color if we have space allocated
    if (ctx->colors && ctx->values_read < ctx->array_capacity)
        ctx->colors[ctx->values_read] = color;

    parser->position = pos;
    return true;
}

// Forward declarations for helpers
bool load_file_into_parser(t_parser *parser, const char *filename);


bool  parse_value(t_parser *parser, t_format_type format, t_token *token)
{
    size_t pos = parser->position;
    int value = 0;
    bool negative = false;
    bool has_digits = false;

    // Handle sign
    if (pos < parser->buffer_size && parser->buffer[pos] == '-')
    {
        negative = true;
        pos++;
    }
    else if (pos < parser->buffer_size && parser->buffer[pos] == '+')
    {
        pos++;
    }

    // Parse digits based on format
    switch (format)
    {
        case FORMAT_DECIMAL:
        case FORMAT_NUMBER:
            while (pos < parser->buffer_size && parser->buffer[pos] >= '0' && parser->buffer[pos] <= '9')
            {
                value = value * 10 + (parser->buffer[pos] - '0');
                pos++;
                has_digits = true;
            }
            break;

        case FORMAT_OCTAL:
            while (pos < parser->buffer_size && parser->buffer[pos] >= '0' && parser->buffer[pos] <= '7')
            {
                value = value * 8 + (parser->buffer[pos] - '0');
                pos++;
                has_digits = true;
            }
            break;

        default:
            return false;
    }

    if (!has_digits)
        return false;

    token->attribute = ATT_VALUE;
    token->format = format;
    token->data.int_val = negative ? -value : value;
    token->position = parser->position;
    token->valid = true;

    parser->position = pos;
    parser->context->current_z = (float)(negative ? -value : value);

    return true;
}


t_ctx_data *create_parser_context(uint32_t attributes)
{
    t_ctx_data *ctx = malloc(sizeof(t_ctx_data));
    if (!ctx)
        return NULL;

    memset(ctx, 0, sizeof(t_ctx_data));
    ctx->current_color = 0xffffff;
    ctx->array_capacity = 64;

    if (attributes & ATT_VALUE) {
        ctx->points = malloc(ctx->array_capacity * sizeof(float));
        if (!ctx->points) {
            free(ctx);
            return NULL;
        }
    }
    if (attributes & ATT_COLOR) {
        ctx->colors = malloc(ctx->array_capacity * sizeof(uint32_t));
        if (!ctx->colors) {
            free(ctx->points);
            free(ctx);
            return NULL;
        }
    }
    if (attributes & (ATT_NORMAL | ATT_TEXTURE | ATT_FACE | ATT_HEADER | ATT_COMMENT | ATT_CUSTOM)) {
        ctx->strings = malloc(ctx->array_capacity * sizeof(char*));
        if (!ctx->strings) {
            free(ctx->points);
            free(ctx->colors);
            free(ctx);
            return NULL;
        }
    }
    
    return ctx;
}