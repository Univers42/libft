/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:32:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 20:30:22 by dlesieur         ###   ########.fr       */
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

typedef struct s_executable_node
{
	int	intfd;
	int	outfd;
	t_ast_node *node;
	t_vec	redirs;
	bool	modify_parent_context;
}	t_executable_node;

void expand_env_vars(t_vec *env, t_ast_node *node);
t_vec split_words(t_ast_node *node, t_vec *env);

#endif