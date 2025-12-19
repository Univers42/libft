/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:38:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 04:19:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "ft_readline.h"
#include "ft_glob.h"
#include "expander.h"

typedef struct s_exec t_exec;
typedef struct s_exec_node
{
    int         infd;
    int         outfd;
    int         next_infd;
   // t_ast_node  *node;
    t_vec       redirs;
    bool           modify_parent_context;
}   t_exec_node;

typedef struct s_redirs
{
    bool    direction_in;
    int     fd;
    char    *fname;
    bool    should_update;
}   t_redirs;

#endif
