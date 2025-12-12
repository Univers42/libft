/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:38:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 00:15:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "ft_readline.h"
#include "ft_glob.h"

typedef struct s_exec t_exec;
typedef struct s_executable_node
{
    int         infd;
    int         outfd;
    int         next_infd;
    t_ast_node  *node;
    t_vec       redirs;
    bool           modify_parent_context;
}   t_executable_node;

#endif