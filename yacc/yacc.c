/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yacc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 14:40:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 05:17:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_yacc.h"

/**
 * Singleton accessor for the default yacc parser
 * Returns either the provided pointer or the static singleton
 */
t_yacc *get_yacc(t_yacc *maybe)
{
    static t_yacc singleton = {0};
    
    if (maybe)
        return maybe;
    return &singleton;
}

/**
 * Initialize the yacc parser
 * Can be called with NULL for singleton or with a custom structure
 */
int yacc_init(t_yacc *yacc)
{
    t_yacc *y = get_yacc(yacc);
    
    ft_assert(y != NULL);
    y->input = (t_dyn_str){0};
    y->pos = (t_position){0};
    y->options = (t_yopt){0};
    vec_init(&y->rules, &(t_vec_config){
        .elem_size = sizeof(t_yrule),
        .initial_capacity = 64,
        .type_mask = VEC_TYPE_PLAIN
    });
    return 0;
}

/**
 * Add a rule to the yacc parser
 * Works with both singleton and custom structures
 */
void yacc_add_rule(t_yacc *yacc, t_match_fn match, t_token_type type, t_trans_fn trans)
{
    t_yacc *y = get_yacc(yacc);
    t_yrule rule;

    ft_assert(y != NULL);
    rule.match = match;
    rule.type = type;
    rule.transform = trans;
    vec_push(&y->rules, &rule);
}


/**
 * Skip whitespace in the input
 */
void yacc_skip_whitespace(t_yacc *yacc)
{
    ft_assert(yacc != NULL);
    while (yacc->input.pos < yacc->input.len && 
           ft_isspace(dyn_str_idx(&yacc->input, yacc->input.pos)))
        yacc->input.pos++;
}

/**
 * Update position tracking (line, column, index)
 */
void yacc_update_position(t_yacc *yacc, size_t length)
{
    size_t i;
    
    ft_assert(yacc != NULL);
    i = 0;
    while (i < length && yacc->pos.idx < yacc->input.len)
    {
        if (dyn_str_idx(&yacc->input, yacc->pos.idx) == '\n')
        {
            yacc->pos.line++;
            yacc->pos.column = 0;
        }
        else
            yacc->pos.column++;
        yacc->pos.idx++;
        i++;
    }
}

/**
 * Match functions - all take t_dyn_str pointer and use input->pos
 */

bool match_comment(t_dyn_str *input)
{
    size_t len;
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || dyn_str_idx(input, pos) != '#')
        return false;
    len = 1;
    while (pos + len < input->len && dyn_str_idx(input, pos + len) != '\n')
        len++;
    /* consume */
    input->pos = pos + len;
    return true;
}

bool match_identifier(t_dyn_str *input)
{
    size_t len;
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len)
        return false;
    if (!ft_isalpha(dyn_str_idx(input, pos)) && dyn_str_idx(input, pos) != '_')
        return false;
    len = 1;
    while (pos + len < input->len &&
           (ft_isalnum(dyn_str_idx(input, pos + len)) || 
            dyn_str_idx(input, pos + len) == '_'))
        len++;
    input->pos = pos + len;
    return true;
}

bool match_number(t_dyn_str *input)
{
    size_t len;
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || !ft_isdigit(dyn_str_idx(input, pos)))
        return false;
    len = 0;
    while (pos + len < input->len && ft_isdigit(dyn_str_idx(input, pos + len)))
        len++;
    if (pos + len < input->len && dyn_str_idx(input, pos + len) == '.')
    {
        len++;
        while (pos + len < input->len && ft_isdigit(dyn_str_idx(input, pos + len)))
            len++;
    }
    input->pos = pos + len;
    return true;
}

bool match_sqstring(t_dyn_str *input)
{
    size_t len;
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || dyn_str_idx(input, pos) != '\'')
        return false;
    len = 1;
    while (pos + len < input->len && dyn_str_idx(input, pos + len) != '\'')
        len++;
    if (pos + len < input->len && dyn_str_idx(input, pos + len) == '\'')
    {
        len++; /* include closing quote */
        input->pos = pos + len;
        return true;
    }
    return false;
}

bool match_dqstring(t_dyn_str *input)
{
    size_t len;
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || dyn_str_idx(input, pos) != '"')
        return false;
    len = 1;
    while (pos + len < input->len && dyn_str_idx(input, pos + len) != '"')
    {
        if (dyn_str_idx(input, pos + len) == '\\' && pos + len + 1 < input->len)
            len += 2;
        else
            len++;
    }
    if (pos + len < input->len && dyn_str_idx(input, pos + len) == '"')
    {
        len++;
        input->pos = pos + len;
        return true;
    }
    return false;
}

bool match_variable(t_dyn_str *input)
{
    size_t len;
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || dyn_str_idx(input, pos) != '$')
        return false;
    len = 1;
    if (pos + len < input->len &&
        (ft_isalpha(dyn_str_idx(input, pos + len)) ||
         dyn_str_idx(input, pos + len) == '_'))
    {
        len++;
        while (pos + len < input->len &&
               (ft_isalnum(dyn_str_idx(input, pos + len)) ||
                dyn_str_idx(input, pos + len) == '_'))
            len++;
        input->pos = pos + len;
        return true;
    }
    return false;
}

bool match_keyword(t_dyn_str *input)
{
    const char *keywords[] = {
        "if", "then", "else", "elif", "fi", "for", "while", "do", "done",
        "case", "esac", "in", "function", NULL
    };
    size_t i;
    size_t len;
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len)
        return false;
    i = 0;
    while (keywords[i])
    {
        len = ft_strlen(keywords[i]);
        if (pos + len <= input->len &&
            ft_strncmp(input->buff + pos, keywords[i], len) == 0 &&
            (pos + len >= input->len ||
             (!ft_isalnum(dyn_str_idx(input, pos + len)) &&
              dyn_str_idx(input, pos + len) != '_')))
        {
            input->pos = pos + len;
            return true;
        }
        i++;
    }
    return false;
}

bool match_operator(t_dyn_str *input)
{
    const char *operators[] = {
        "==", "!=", "<=", ">=", "++", "--", "+=", "-=", NULL
    };
    size_t i;
    size_t len;
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len)
        return false;
    i = 0;
    while (operators[i])
    {
        len = ft_strlen(operators[i]);
        if (pos + len <= input->len &&
            ft_strncmp(input->buff + pos, operators[i], len) == 0)
        {
            input->pos = pos + len;
            return true;
        }
        i++;
    }
    if (ft_strchr("+-*/<>=!&|", dyn_str_idx(input, pos)))
    {
        input->pos = pos + 1;
        return true;
    }
    return false;
}

bool match_pipe(t_dyn_str *input)
{
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || dyn_str_idx(input, pos) != '|')
        return false;
    if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '|')
        return false;
    input->pos = pos + 1;
    return true;
}

bool match_and(t_dyn_str *input)
{
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || dyn_str_idx(input, pos) != '&')
        return false;
    if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '&')
    {
        input->pos = pos + 2;
        return true;
    }
    return false;
}

bool match_or(t_dyn_str *input)
{
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || dyn_str_idx(input, pos) != '|')
        return false;
    if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '|')
    {
        input->pos = pos + 2;
        return true;
    }
    return false;
}

bool match_ampersand(t_dyn_str *input)
{
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len || dyn_str_idx(input, pos) != '&')
        return false;
    /* if next is '&', it's handled by match_and */
    if (pos + 1 < input->len && dyn_str_idx(input, pos + 1) == '&')
        return false;
    input->pos = pos + 1;
    return true;
}

bool match_word(t_dyn_str *input)
{
    size_t pos = input->pos;

    ft_assert(input && input->buff);
    if (pos >= input->len)
        return false;
    /* don't start word on whitespace or metacharacters */
    if (ft_isspace(dyn_str_idx(input, pos)))
        return false;
    if (dyn_str_idx(input, pos) == '|' || dyn_str_idx(input, pos) == '&' || dyn_str_idx(input, pos) == ';' || dyn_str_idx(input, pos) == '<' || dyn_str_idx(input, pos) == '>' || dyn_str_idx(input, pos) == '\n' || dyn_str_idx(input, pos) == '\'' || dyn_str_idx(input, pos) == '"' || dyn_str_idx(input, pos) == '$' )
        return false;
    /* consume until delimiter */
    while (input->pos < input->len)
    {
        char c = dyn_str_idx(input, input->pos);
        if (ft_isspace(c) || c == '|' || c == '&' || c == ';' || c == '<' || c == '>' || c == '\n' || c == '\'' || c == '"' || c == '$')
            break;
        input->pos++;
    }
    return true;
}

/**
 * Get next token from the input
 */
t_ytoken *yacc_next_token(t_yacc *yacc)
{
    size_t i;
    t_yrule *rule;
    t_ytoken *token;
    size_t before;
    size_t consumed;

    ft_assert(yacc != NULL);
    if (yacc->options.skip_whitespace)
        yacc_skip_whitespace(yacc);
    if (yacc->input.pos >= yacc->input.len)
        return NULL;
    i = 0;
    while (i < yacc->rules.len)
    {
        rule = (t_yrule *)vec_idx(&yacc->rules, i);
        before = yacc->input.pos;
        if (rule && rule->match(&yacc->input))
        {
            /* compute how many bytes consumed by matcher and update position tracking */
            consumed = yacc->input.pos - before;
            if (consumed == 0)
            {
                /* avoid infinite loop: consume one byte */
                yacc->input.pos = before + 1;
                consumed = 1;
            }
            yacc_update_position(yacc, consumed);
            token = malloc(sizeof(t_ytoken));
            if (!token)
                return NULL;
            token->type = rule->type;
            token->position = yacc->pos;
            token->input = yacc->input;
            if (rule->transform)
                rule->transform(token->input);
            return token;
        }
        i++;
    }
    return NULL;
}

/**
 * Tokenize entire input
 */
t_ytoken **yacc_tokenize_all(t_yacc *yacc, int *token_count)
{
    int cap;
    int count;
    t_ytoken **tokens;
    t_ytoken *token;

    ft_assert(yacc && token_count);
    cap = 64;
    count = 0;
    tokens = malloc(sizeof(t_ytoken *) * cap);
    if (!tokens)
        return NULL;
    while (yacc->pos.idx < yacc->input.len)
    {
        token = yacc_next_token(yacc);
        if (!token)
            break;
        if (count >= cap)
        {
            cap *= 2;
            tokens = realloc(tokens, sizeof(t_ytoken *) * cap);
            if (!tokens)
                return NULL;
        }
        tokens[count++] = token;
    }
    *token_count = count;
    return tokens;
}

/**
 * Create a yacc parser with custom options
 */
t_yacc *yacc_create(t_yopt options)
{
    t_yacc *yacc;

    yacc = malloc(sizeof(t_yacc));
    if (!yacc)
        return NULL;
    yacc_init(yacc);
    yacc->options = options;
    return yacc;
}

/**
 * Destroy a yacc parser
 */
void yacc_destroy(t_yacc *yacc)
{
    if (!yacc)
        return;
    dyn_str_free(&yacc->input);
    vec_destroy(&yacc->rules);
    free(yacc);
}

/**
 * Setup default lexer rules
 */
void yacc_setup_deft_lexer(t_yacc *yacc)
{
    t_yacc *y = get_yacc(yacc);
    
    /* order matters: most specific rules first */
    yacc_add_rule(y, match_comment, TOKEN_HASH, NULL);    
    yacc_add_rule(y, match_keyword, TOKEN_KEYWORDS, NULL);
    yacc_add_rule(y, match_variable, TOKEN_VARIABLE, NULL);
    yacc_add_rule(y, match_number, TOKEN_NUMBER, NULL);
    yacc_add_rule(y, match_sqstring, TOKEN_SQUOTE_STRING, NULL);
    yacc_add_rule(y, match_dqstring, TOKEN_DQUOTE_STRING, NULL);
    yacc_add_rule(y, match_or, TOKEN_LOGICAL_OR, NULL);
    yacc_add_rule(y, match_and, TOKEN_LOGICAL_AND, NULL);
    yacc_add_rule(y, match_pipe, TOKEN_PIPE, NULL);
    yacc_add_rule(y, match_ampersand, TOKEN_AMPERSAND, NULL);
    yacc_add_rule(y, match_operator, TOKEN_OPERATOR, NULL);
    yacc_add_rule(y, match_identifier, TOKEN_IDENTIFIER, NULL);
    yacc_add_rule(y, match_word, TOKEN_WORD, NULL);
}

