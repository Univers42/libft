/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yacc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 14:40:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/18 21:22:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_yacc.h"

/**
 * @param personnalized say to the function if people want to put or not their
 * own structure
 */

t_yacc *get_yacc(bool personnalized)
{
    static t_yacc *singleton = NULL;

    if (personnalized)
        return NULL;
    if (singleton)
        return singleton;
    singleton = malloc(sizeof(t_yacc));
    if (!singleton)
        return NULL;
    singleton->input = (t_dyn_str){0};
    singleton->pos = (t_position){0};
    singleton->options = (t_yopt){0};
    vec_init(&singleton->rules, &(t_vec_config){
        .elem_size = sizeof(t_yrule),
        .initial_capacity = 64,
        .type_mask = VEC_TYPE_PLAIN
    });
    return singleton;
}

// For default singleton
void add_rule(match_fn match, t_token_type type, trans_fn trans)
{
    t_yacc *yacc = get_yacc(false);
    t_yrule rule;

    if (!yacc)
        return;
    rule.match = match;
    rule.type = type;
    rule.transform = trans;
    vec_push(&yacc->rules, &rule);
}

// For user-provided yacc structure
void dft_add_rule(void *structure, match_fn match, t_token_type type, trans_fn trans)
{
    t_yacc *yacc = (t_yacc *)structure;
    t_yrule rule;

    if (!yacc)
        return;
    rule.match = match;
    rule.type = type;
    rule.transform = trans;
    vec_push(&yacc->rules, &rule);
}

void	setup_deft_lexer()
{
	add_rule(match_comment, TOKEN_HASH, NULL);	
	add_rule(match_keyword, TOKEN_KEYWORDS, NULL);
	add_rule(match_variable, TOKEN_VARIABLE, NULL);
	add_rule(match_number, TOKEN_NUMBER, NULL);
	add_rule(match_sqstring, TOKEN_SQUOTE_STRING, NULL);
	add_rule(match_dqstring, TOKEN_DQUOTE_STRING, NULL);
	add_rule(match_or, TOKEN_LOGICAL_OR, NULL);
	add_rule(match_pipe, TOKEN_PIPE, NULL);
	add_rule(match_identifier, TOKEN_IDENTIFIER, NULL);
	add_rule(match_operator, TOKEN_OPERATOR, NULL);
	add_rule(match_and, TOKEN_LOGICAL_AND, NULL);
}

void 	update_position(t_yacc *yacc, int length)
{
	size_t i;
	t_dyn_str *input;

	input = get_dyn_str(NULL);
	while (i < length)
	{
		if (dyn_str_check_at(yacc->input. , NULL, "\n", CHAR_SIZE))
		{
			i
			yacc->pos.column = 0;
		}
		else
			yacc->pos.column++
	}
}

void	yacc_skip_whitespace(t_yacc *yacc)
{
	while (yacc->input[yacc->pos] && ft_isspace(yacc->input[yacc->pos]))
		yacc->pos++;	
}

t_yacc	*yacc_next_token(t_yacc *yacc)
{
	ft_assert(yacc != NULL);
	if (yacc->options.skip_whitespace)
	{
		while (dyn_str_check_at(yacc->input.pos, ft_isspace, NULL, 0))
			yacc->input.pos++;
	}
}

t_yacc	**yacc_tokenize_all(t_yacc *yacc, int *token_count)
{
	int		cap;
	int		count;
	t_yacc	**toks;
	t_yacc	*tok;

	toks = malloc(sizeof(t_yacc *) * 64);
	tok = yacc_next_token();
	while ((t_token = lexer_next_token(cd)))
}


/**
 * in implementation of match we have to be concerned about the
 * the consumption of data
 * */
bool	match_sqstring(t_dyn_str *input)
{
	
}

bool	match_dqstring(t_dyn_str *input)
{
		
}

bool	match_identifier(t_dyn_str *input)
{

}

bool	match_number(t_dyn_str *input)
{

}

bool	match_string(t_dyn_str *input)
{
	char quote;
	int	len;

	quote = dyn_str_idx(input, input->pos);
	while (dyn_str_check_at(input->pos, NULL, &quote, CHAR_SIZE))
	{

	}
		len++;
}

bool	match_keyword(t_dyn_str *input)
{
	size_t	i;
	const char *keywords[] = {
		"if", "then", "else", "elif", "fi", "for", "while", "do", "done",
		"case", "esac", "in", "function", NULL
	};

	i = 0;
	while (keywords[i])
	{
		size_t	len = ft_strlen(keywords[i]);
		if (dyn_str_check_at(input->pos, NULL, (char *)keywords[i], len))
		{
			if (dyn_str_check_at(input->pos + len, NULL, "\0", 1) ||
				(!dyn_str_check_at(input->pos + len, ft_isalnum, NULL, 0) &&
				 !dyn_str_check_at(input->pos + len, NULL, "_", 1)))
			{
				dyn_str_uppdate_len(len);
				return (true);
			}
		}
		i++;
	}
	return (false);
}

bool	match_operator(t_dyn_str *input)
{
    size_t	i;
    const char *operators[] = {
        "==", "!=", "<=", ">=", "<", ">", "+", "-", "*", "/", "%",
        "=", "+=", "-=", "*=", "/=", "%=", NULL
    };

	i = 0;
	while (operators[i])
	{
		size_t	len = ft_strlen(operators[i]);
		if (dyn_str_check_at(input->pos, NULL, (char *)operators[i], len))
			return (dyn_str_uppdate_len(len), true);
		i++;
	}
	if (ft_strchr("+-*/<>=!&|", input->buff[input->pos]))
		return (dyn_str_uppdate_len(1), true);
	return (false);
}

bool	match_pipe(t_dyn_str *input)
{
    if (dyn_str_check_at(input->pos, NULL, "|", 1))
    {
        dyn_str_uppdate_len(1);
        return (true);
    }
    return (false);
}

bool	match_and(t_dyn_str *input)
{
    if (dyn_str_check_at(input->pos, NULL, "&", 1) &&
        dyn_str_check_at(input->pos + 1, NULL, "&", 1))
        return (dyn_str_uppdate_len(2), true);
    return (false);
}

bool	match_or(t_dyn_str *input)
{
    if (dyn_str_check_at(input->pos, NULL, "|", 1) &&
        dyn_str_check_at(input->pos + 1, NULL, "|", 1))
        return (dyn_str_uppdate_len(2), true);
    return (false);
}

t_dyn_str	*get_dyn_str(t_dyn_str *maybe)
{
	static	t_dyn_str  str	= {0};

	if (maybe)
		return (maybe);
	return (&str);
}

bool match_variable(t_dyn_str *input)
{
    int len;

    ft_assert(input != NULL && input->buff != NULL);

    if (input->pos >= input->len)
        return (false);
    if (!dyn_str_check_at(input->pos, NULL, "$", 1))
        return (false);
    len = 1;
    if (input->pos + len < input->len &&
        (dyn_str_check_at(input->pos + len, ft_isalpha, NULL, 0) ||
         dyn_str_check_at(input->pos + len, NULL, "_", 1)))
    {
        len++;
        while (input->pos + len < input->len &&
               (dyn_str_check_at(input->pos + len, ft_isalnum, NULL, 0) ||
                dyn_str_check_at(input->pos + len, NULL, "_", 1)))
            len++;
        dyn_str_uppdate_len(len);
        return (true);
    }
    return (false);
}

bool match_comment(t_dyn_str *input)
{
    int len = 0;

    if (!input || !input->buff || input->pos >= input->len)
        return false;
    if (input->buff[input->pos] != '#')
        return false;
    len = 1;
    while ((input->pos + len) < input->len && input->buff[input->pos + len] != '\n')
        len++;
    input->pos += len;
    return true;
}

bool	match_and(t_dyn_str *)
{
	
}

t_yacc *yacc_create(t_yopt options)
{
    t_yacc *yacc = malloc(sizeof(t_yacc));
    if (!yacc)
        return NULL;
    dyn_str_init(&yacc->input);
    yacc->pos = (t_position){0};
    yacc->options = options;
    vec_init(&yacc->rules, (t_vec_config){
        .elem_size = sizeof(t_yrule),
        .initial_capacity = 64,
        .type_mask = VEC_TYPE_PTR
    });
    return yacc;
}

void	yacc_destroy(t_yacc *yacc)
{
	if (!yacc)
		return ;
	vec_clear(&yacc->rules);
	free(yacc);
}

int	main(void)
{
	const t_yopt	options = {
		.skip_whitespace = true,
		.track_position = true,
		.error_on_unknown = false
	};
	t_vec	lexer;
	t_token	**tokens;
	int		count;

	vec_init(&lexer, (t_vec_config){.elem_size = sizeof(t_yacc), .initial_capacity = 64, .type_mask = VEC_TYPE_PTR});
	if (!lexer)
		return (1);
	setup_shell_lexer();
	printf("=== Shell command Lexer ===\n");
	for (int i = 0; i < count; i++)
	{
		token_print(tokens[i]);
		token_destroy(tokens[i]);
	}
	free(tokens);
	printf("\n === variables ===\n");
	vec_clear(&lexer);
	lexer_set_input(lexer, "x = $HOME && echo $USER");
	vecc_destroy(&lexer);
	return  (0);
}