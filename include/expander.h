/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:32:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 16:46:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

#include "ft_glob.h"
#include "ft_string.h"
#include "ft_memory.h"
#include "ds.h"
#include "var.h"
#include "ipc.h"

typedef s_expander
{
	bool    found_first;
	bool    export;
	int		exit_status;
	t_ast_node *curr;
	size_t	i;
}t_expander;

typedef struct s_expander_simple_cmd
{
	bool		found_first;
	bool		export;
	int			exit_stat;
	t_ast_node	*curr;
	size_t		i;
}	t_expander_simple_cmd;

typedef struct s_executable_node
{
	int	intfd;
	int	outfd;
	t_ast_node *node;
	t_vec	redirs;
	bool	modify_parent_context;
}	t_executable_node;

typedef struct executable_cmd_s
{
	t_vec		pre_assigns;
	t_vec		argv;
	char		*fname;
}	t_executable_cmd;

t_dyn_str	word_to_string(t_ast_node node);
t_dyn_str	word_to_hrdoc_string(t_ast_node node);
t_env	assignment_to_env(t_ast_node *node);
void	assignment_word_to_word(t_ast_node *node);
bool	create_redir(t_token_type tt, char *fname, t_redir *ret);
void print_redir_err(char **context, t_tok full_token, char *expanded_name);
t_tok	get_old_token(t_ast_node word);
int	redirect_from_ast_redir(char **context, t_ast_node *curr, int *redir_idx);
bool    is_export(t_ast_node word);

void expand_env_vars(t_vec *env, t_ast_node *node);
t_vec split_words(t_ast_node *node, t_vec *env);
void expand_word(t_ast_node *node, t_vec *args, bool keep_as_one, t_vec *env);
void expand_node_glob(t_ast_node *node, t_vec *args, bool keep_as_one);
t_tok get_old_token(t_ast_node word);
char *expand_word_single(t_vec *env, t_ast_node *curr);

#endif
