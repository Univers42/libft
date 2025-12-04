/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:03:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 23:23:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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

void print_token_debug(t_token *token)
{
    const char *name = get_token_name(token->type);

    printf("%-30s | len: %3d | ", name, token->len);
    if (token->len > 0)
        printf("'%.*s'", token->len, token->start);
    else
        printf("(empty)");
    printf("\n");
}

void print_tokens(t_deque *tokens)
{
    t_token *curr;
    int i;

    i = 0;
    printf("\n========== TOKEN LIST ==========\n");
    printf("Total tokens: %d\n", (int)tokens->len);
    printf("================================\n");
    while (i < (int)tokens->len)
    {
        curr = (t_token *)deque_idx(tokens, i);
        printf("[%3d] ", i);
        print_token_debug(curr);
        i++;
    }
    printf("================================\n\n");
}

void print_token_summary(t_deque *tokens)
{
    t_token *curr;
    int i;
    int type_counts[TOKEN_SENTINEL] = {0};

    i = 0;
    while (i < (int)tokens->len)
    {
        curr = (t_token *)deque_idx(tokens, i);
        if (curr->type < TOKEN_SENTINEL)
            type_counts[curr->type]++;
        i++;
    }

    printf("\n========== TOKEN SUMMARY ==========\n");
    for (i = 0; i < TOKEN_SENTINEL; i++)
    {
        if (type_counts[i] > 0)
            printf("%-30s: %d\n", get_token_name(i), type_counts[i]);
    }
    printf("===================================\n\n");
}