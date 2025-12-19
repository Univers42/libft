/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:03:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 05:06:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ft_colors.h"

const char *get_token_name(t_token_type type)
{
    int i;

    i = 0;
    while (tok[i].rep_name != NULL)
    {
        if (tok[i].token == type)
            return (tok[i].rep_name);
        i++;
    }
    return ("UNKNOWN");
}

/* Choose a color based on token category (very lightweight heuristic). */
static const char *token_color(t_token_type type)
{
    if (is_redir_token(type))
        return YELLOW_TERM;
    if (is_logical_token(type) || type == TOKEN_PIPE)
        return CYAN_TERM;
    if (is_keyword_token(type))
        return MAGENTA_TERM;
    if (type == TOKEN_WORD || type == TOKEN_IDENTIFIER || type == TOKEN_NUMBER)
        return GREEN_TERM;
    if (type == TOKEN_ERR)
        return RED_TERM BOLD_TERM;
    return BLUE_TERM;
}

void print_token_debug(t_token *token)
{
    const char *name = get_token_name(token->type);
    const char *color = token_color(token->type);

    /* Format: "TYPE (28 chars) | len:NNN | 'lexeme'" */
    printf("%s%-30s%s │ %3d │ ", color, name, RESET_TERM, token->len);
    if (token->len > 0)
        printf("%s'%.*s'%s", GREY_TERM, token->len, token->start, RESET_TERM);
    else
        printf("%s(empty)%s", GREY_TERM, RESET_TERM);
    printf("\n");
}

void print_tokens(t_deque *tokens)
{
    t_token *curr;
    int i;

    i = 0;
    printf("\n");
    printf(CYAN_TERM "╔════════════════════════════════════════════════════════════════════════════╗\n" RESET_TERM);
    printf(CYAN_TERM "║" RESET_TERM BOLD_TERM "                           TOKEN LIST                                       " RESET_TERM CYAN_TERM "║\n" RESET_TERM);
    printf(CYAN_TERM "╚════════════════════════════════════════════════════════════════════════════╝\n" RESET_TERM);
    printf(GREY_TERM "Total tokens: %d\n\n" RESET_TERM, (int)tokens->len);

    printf(CYAN_TERM "╔═════╦════════════════════════════════╦═════╦════════════════════════════════╗\n" RESET_TERM);
    printf(CYAN_TERM "║" RESET_TERM BOLD_TERM " idx " RESET_TERM CYAN_TERM "║" RESET_TERM BOLD_TERM " %-30s " RESET_TERM CYAN_TERM "║" RESET_TERM BOLD_TERM " len " RESET_TERM CYAN_TERM "║" RESET_TERM BOLD_TERM " %-30s " RESET_TERM CYAN_TERM "║\n" RESET_TERM, "type", "lexeme");
    printf(CYAN_TERM "╠═════╬════════════════════════════════╬═════╬════════════════════════════════╣\n" RESET_TERM);

    while (i < (int)tokens->len)
    {
        curr = (t_token *)deque_idx(tokens, i);
        const char *name = get_token_name(curr->type);
        const char *color = token_color(curr->type);

        /* Build lexeme string */
        char lexeme_buf[128];
        if (curr->len > 0)
        {
            snprintf(lexeme_buf, sizeof(lexeme_buf), "'%.*s'",
                     curr->len > 80 ? 80 : curr->len, curr->start);
        }
        else
        {
            snprintf(lexeme_buf, sizeof(lexeme_buf), "(empty)");
        }

        /* Print row with proper padding */
        printf(CYAN_TERM "║" RESET_TERM " %3d " CYAN_TERM "║" RESET_TERM " %s%-30s%s " CYAN_TERM "║" RESET_TERM " %3d " CYAN_TERM "║" RESET_TERM " %s%-30s%s" CYAN_TERM "║\n" RESET_TERM,
               i, color, name, RESET_TERM, curr->len, GREY_TERM, lexeme_buf, RESET_TERM);
        i++;
    }
    printf(CYAN_TERM "╚═════╩════════════════════════════════╩═════╩════════════════════════════════╝\n" RESET_TERM);
    printf("\n");
}

void print_token_summary(t_deque *tokens)
{
    t_token *curr;
    int i;
    int type_counts[TOKEN_SENTINEL] = {0};
    int total = (int)tokens->len;

    i = 0;
    while (i < (int)tokens->len)
    {
        curr = (t_token *)deque_idx(tokens, i);
        if (curr->type < TOKEN_SENTINEL)
            type_counts[curr->type]++;
        i++;
    }

    printf("\n");
    printf(MAGENTA_TERM "╔════════════════════════════════════════════════╗\n" RESET_TERM);
    printf(MAGENTA_TERM "║" RESET_TERM BOLD_TERM "              TOKEN SUMMARY                     " RESET_TERM MAGENTA_TERM "║\n" RESET_TERM);
    printf(MAGENTA_TERM "╚════════════════════════════════════════════════╝\n" RESET_TERM);
    printf(GREY_TERM "Total tokens: %d\n\n" RESET_TERM, total);

    printf(MAGENTA_TERM "╔════════════════════════════════╦═══════╗\n" RESET_TERM);
    printf(MAGENTA_TERM "║" RESET_TERM BOLD_TERM " %-30s " RESET_TERM MAGENTA_TERM "║" RESET_TERM BOLD_TERM " count " RESET_TERM MAGENTA_TERM "║\n" RESET_TERM, "type");
    printf(MAGENTA_TERM "╠════════════════════════════════╬═══════╣\n" RESET_TERM);

    for (i = 0; i < TOKEN_SENTINEL; i++)
    {
        if (type_counts[i] > 0)
        {
            const char *name = get_token_name(i);
            const char *color = token_color(i);
            printf(MAGENTA_TERM "║" RESET_TERM " %s%-30s%s " MAGENTA_TERM "║" RESET_TERM " %5d " MAGENTA_TERM "║\n" RESET_TERM,
                   color, name, RESET_TERM, type_counts[i]);
        }
    }
    printf(MAGENTA_TERM "╚════════════════════════════════╩═══════╝\n" RESET_TERM);
    printf("\n");
}
