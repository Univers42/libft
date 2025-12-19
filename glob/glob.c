/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:47:05 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:04:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/glob.h"
#include "ds.h"

// size_t matches_pattern(char *name, t_vec *patt, size_t offset, bool first);
// void match_dir(t_vec *args, t_vec *glob, const char *path, size_t offset);

// static char	*tt_to_str_p2(t_token_type tt)
// {
// 	if (tt == TOKEN_END)
// 		return ("TT_END");
// 	if (tt == TOKEN_WORD)
// 		return ("TT_WORD");
// 	if (tt == TOKEN_REDIR_FD_IN)
// 		return ("TT_REDIRECT_LEFT");
// 	if (tt == TOKEN_REDIR_FD_OUT)
// 		return ("TT_REDIRECT_RIGHT");
// 	if (tt == TOKEN_REDIR_APPEND)
// 		return ("TT_APPEND");
// 	if (tt == TOKEN_PIPE)
// 		return ("TT_PIPE");
// 	if (tt == TOKEN_LEFT_PAREN)
// 		return ("TT_BRACE_LEFT");
// 	if (tt == TOKEN_RIGHT_PAREN)
// 		return ("TT_BRACE_RIGHT");
// 	if (tt == TOKEN_LOGICAL_OR)
// 		return ("TT_OR");
// 	if (tt == TOKEN_LOGICAL_AND)
// 		return ("TT_AND");
// 	ft_assert(false);
// 	return (0);
// }

// static char	*tt_to_str(t_token_type tt)
// {
// 	if (tt == TOKEN_SEMICOLON)
// 		return ("TT_SEMICOLON");
// 	if (tt == TOKEN_REDIR_HEREDOC)
// 		return ("TT_HEREDOC");
// 	if (tt == TOKEN_NEWLINE)
// 		return ("TT_NEWLINE");
// 	if (tt == TOKEN_SQUOTE_STRING)
// 		return ("TT_QWORD");
// 	if (tt == TOKEN_DQUOTE_STRING)
// 		return ("TT_DQWORD");
// 	if (tt == TOKEN_ENVVAR)
// 		return ("TT_ENVVAR");
// 	if (tt == TOKEN_DQENVVAR)
// 		return ("TT_DQENVVAR");
// 	return (tt_to_str_p2(tt));
// }

// t_dyn_str word_to_string(t_ast_node node)
// {
// 	t_ast_node *curr_node;
// 	t_token curr;
// 	t_dyn_str s;
// 	size_t i;

// 	dyn_str_init(&s);
// 	i = 0;
// 	while (i < node.children.len)
// 	{
// 		curr_node = (t_ast_node *)vec_idx(&node.children, i);
// 		ft_assert(curr_node && curr_node->node_type == AST_TOKEN);
// 		ft_assert(curr_node->token != NULL);
// 		curr = *(curr_node->token);
// 		if (curr.type == TOKEN_WORD || curr.type == TOKEN_SQUOTE_STRING || curr.type == TOKEN_DQUOTE_STRING || curr.type == TOKEN_DQENVVAR || curr.type == TOKEN_ENVVAR)
// 		{
// 			ft_assert(curr.start || !curr.len);
// 			dyn_str_pushnstr(&s, curr.start, curr.len);
// 		}
// 		else
// 		{
// 			ft_printf("got unexpected: %s\n", tt_to_str(curr.type));
// 			ft_assert("Unreachable" == 0);
// 		}
// 		i++;
// 	}
// 	return (s);
// }

// static bool copy_cstr(void *dst, const void *src)
// {
// 	char *s;

// 	s = *(char **)src;
// 	if (!s)
// 	{
// 		*(char **)dst = NULL;
// 		return (true);
// 	}
// 	*(char **)dst = ft_strdup(s);
// 	return (*(char **)dst != NULL);
// }

// static void free_cstr(void *elem)
// {
// 	char *s;

// 	s = *(char **)elem;
// 	free(s);
// }

// static bool glob_copy(void *dst, const void *src)
// {
// 	const t_glob *s = (const t_glob *)src;
// 	t_glob *d = (t_glob *)dst;
// 	*d = *s;
// 	if (s->start)
// 	{
// 		d->start = ft_strdup(s->start);
// 		if (!d->start)
// 			return (false);
// 	}
// 	return (true);
// }

// static void glob_free(void *elem)
// {
// 	t_glob *g = (t_glob *)elem;

// 	free(g->start);
// }

// void get_next_path(t_dyn_str *next_path, const char *path, const char *fname)
// {
// 	dyn_str_init(next_path);
// 	dyn_str_pushstr(next_path, path);
// 	dyn_str_pushstr(next_path, fname);
// }

// char *get_curr_path(const char *path)
// {
// 	if (*path)
// 		return ((char *)path);
// 	else
// 		return (".");
// }

// int process_dir(t_dir_matcher matcher)
// {
// 	struct dirent *diren;
// 	t_dyn_str next_path;
// 	size_t res;

// 	diren = readdir(matcher.dir);
// 	if (!diren)
// 		return (0);
// 	res = matches_pattern(diren->d_name, &matcher.glob, matcher.offset, true);
// 	dyn_str_init(&next_path);
// 	if (res)
// 	{
// 		get_next_path(&next_path, matcher.path, diren->d_name);
// 		if (matcher.glob.len > res)
// 			dyn_str_push(&next_path, '/');
// 		else
// 		{
// 			vec_push(matcher.args, &next_path.buff);
// 			return (free(next_path.buff), 1);
// 		}
// 		match_dir(matcher.args, &matcher.glob, next_path.buff, res + 1);
// 		free(next_path.buff);
// 	}
// 	return (1);
// }

// void match_dir(t_vec *args, t_vec *glob, const char *path, size_t offset)
// {
// 	DIR *dir;
// 	t_dir_matcher matcher;

// 	dir = opendir(get_curr_path(path));
// 	if (glob->len <= offset && dir)
// 	{
// 		vec_push(args, &(char *){ft_strdup(path)});
// 		closedir(dir);
// 		return;
// 	}
// 	if (!dir)
// 		return;
// 	matcher.path = (char *)path;
// 	matcher.dir = dir;
// 	/* fix: copy the vector struct, don't assign pointer to struct */
// 	matcher.glob = *glob;
// 	matcher.offset = offset;
// 	matcher.args = args;
// 	while (!get_g_sig()->should_unwind && process_dir(matcher))
// 		;
// 	closedir(dir);
// }

// static void tokenize_star_glob(t_vec *ret, t_token t, int *i)
// {
// 	(void)t;
// 	while (*i < t.len && t.start[*i] == '*')
// 		(*i)++;
// 	vec_push(ret, &(t_glob){.st = ST_ASTERISK, .start = NULL, .len = 0});
// }

// static bool star_expandable(t_token_type token_type)
// {
// 	if (token_type == TOKEN_SQUOTE_STRING || token_type == TOKEN_DQUOTE_STRING || token_type == TOKEN_DQENVVAR)
// 		return (false);
// 	else if (token_type == TOKEN_WORD || token_type == TOKEN_ENVVAR)
// 		return (true);
// 	else
// 	{
// 		ft_printf("got unexpected: %s\n", tt_to_str(token_type));
// 		ft_assert("Unreachable" == 0);
// 	}
// 	return (false);
// }

// static void tokenize_pattern(t_vec *ret, t_token t, int *i)
// {
// 	bool expandable;
// 	char *start;

// 	expandable = star_expandable(t.type);
// 	start = (char *)(t.start + *i);
// 	while (*i < t.len && t.start[*i] != '/' && (t.start[*i] != '*' || !expandable))
// 		(*i)++;
// 	vec_push(ret, &(t_glob){.st = ST_MATCH_GLOBREP, .start = start, .len = (t.start + *i) - start});
// }

// static void tokenize_word_glob(t_vec *ret, t_token t)
// {
// 	int i;

// 	i = 0;
// 	while (i < t.len)
// 	{
// 		if (t.start[i] == '/')
// 		{
// 			vec_push(ret, &(t_glob){.st = ST_SLASH, .start = NULL, .len = 0});
// 			i++;
// 		}
// 		else if (t.start[i] == '*' && star_expandable(t.type))
// 			tokenize_star_glob(ret, t, &i);
// 		else
// 			tokenize_pattern(ret, t, &i);
// 	}
// }

// static t_vec word_to_glob(t_ast_node word)
// {
// 	size_t i;
// 	t_ast_node *curr_node;
// 	t_token curr_t;
// 	t_vec ret;

// 	i = 0;
// 	vec_init(&ret, &(t_vec_config){
// 					   .initial_capacity = 64,
// 					   .elem_size = sizeof(t_glob),
// 					   .copy_fn = glob_copy,
// 					   .free_fn = glob_free});
// 	while (i < word.children.len)
// 	{
// 		curr_node = (t_ast_node *)vec_idx(&word.children, i);
// 		ft_assert(curr_node && curr_node->node_type == AST_TOKEN);
// 		ft_assert(curr_node->token != NULL);
// 		curr_t = *(curr_node->token);
// 		tokenize_word_glob(&ret, curr_t);
// 		i++;
// 	}
// 	return (ret);
// }

// static int cmp_globs(const void *a, const void *b)
// {
// 	const char *str_a = *(const char **)a;
// 	const char *str_b = *(const char **)b;
// 	return (ft_strcmp(str_a, str_b));
// }

// t_vec expand_word_glob(t_ast_node *word)
// {
// 	t_vec args;
// 	t_vec globs;

// 	vec_init(&args, &(t_vec_config){
// 						.initial_capacity = 64,
// 						.elem_size = sizeof(char *),
// 						.type_mask = VEC_TYPE_PTR,
// 						.copy_fn = copy_cstr,
// 						.free_fn = free_cstr});

// 	globs = word_to_glob(*word);
// 	if (globs.len == 0)
// 	{
// 		char *empty = ft_strdup("");
// 		vec_push(&args, &empty);
// 		vec_destroy(&globs);
// 		return (args);
// 	}
// 	if (((t_glob *)globs.buff)[0].st == ST_SLASH)
// 		match_dir(&args, &globs, "/", 1);
// 	else
// 		match_dir(&args, &globs, "", 0);
// 	if (args.len == 0)
// 	{
// 		t_dyn_str word_str = word_to_string(*word);
// 		vec_push(&args, &word_str.buff);
// 	}
// 	vec_destroy(&globs);
// 	if (!get_g_sig()->should_unwind)
// 		ft_qsort(args.buff, args.len, sizeof(char *), cmp_globs);
// 	else
// 		vec_destroy(&args);
// 	return (args);
// }

// static bool finished_pattern(t_vec patt, size_t offset)
// {
// 	t_glob *curr;

// 	if (patt.len == offset + 1)
// 		return (true);
// 	curr = (t_glob *)patt.buff + offset + 1;
// 	if (curr->st == ST_SLASH)
// 		return (true);
// 	return (false);
// }

// size_t match_g_pattern(char *name, t_vec *patt, size_t offset, bool first)
// {
// 	t_glob *curr;
// 	char *orig_name;

// 	orig_name = name;
// 	while (offset < patt->len && ((t_glob *)patt->buff)[offset].st == ST_MATCH_GLOBREP)
// 	{
// 		curr = (t_glob *)patt->buff + offset;
// 		if (ft_strncmp(curr->start, name, curr->len) != 0)
// 			return (0);
// 		if (finished_pattern(*patt, offset))
// 		{
// 			if (name[curr->len] == 0)
// 				return (offset + 1);
// 			return (0);
// 		}
// 		offset++;
// 		name += curr->len;
// 	}
// 	if (first && (!ft_strcmp(orig_name, "..") || !ft_strcmp(orig_name, ".")) && offset != patt->len && ((t_glob *)patt->buff)[offset].st != ST_SLASH)
// 		return (0);
// 	return (matches_pattern(name, patt, offset, false));
// }

// size_t match_g_asterisk(char *name, t_vec *patt, size_t offset, bool first)
// {
// 	size_t res;

// 	if (first && *name == '.')
// 		return (0);
// 	while (*name)
// 	{
// 		if (!finished_pattern(*patt, offset))
// 		{
// 			res = matches_pattern(name, patt, offset + 1, false);
// 			if (res)
// 				return (res);
// 		}
// 		name++;
// 	}
// 	if (finished_pattern(*patt, offset))
// 		return (offset + 1);
// 	return (0);
// }

// size_t matches_pattern(char *name, t_vec *patt, size_t offset, bool first)
// {
// 	t_glob *curr;

// 	if (!patt || !patt->buff || offset >= patt->len)
// 		return (0);
// 	curr = (t_glob *)patt->buff + offset;
// 	if (curr->st == ST_MATCH_GLOBREP)
// 		return (match_g_pattern(name, patt, offset, first));
// 	else if (curr->st == ST_ASTERISK)
// 		return (match_g_asterisk(name, patt, offset, first));
// 	return (0);
// }
