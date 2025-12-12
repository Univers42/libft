/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 00:15:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 00:25:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "var.h"

static t_status execute_tree_node(t_executable_node *exe, t_status *last_cmd_status_res, char **last_cmd_status_s)
{
    t_status    status;
    t_ast_t     t;

    t = exe->node->node_type;
    status = res_status(0);
    if (t == AST_CMD_PIPELINE)  
        status = execute_pipeline(, exe);
    else if (t == AST_SIMPLE_LIST || t == AST_COMPOUND_LIST)
        status = execute_simple_list();
    else
        ft_assert("Unreachable" == 0);
    set_cmd_status(last_cmd_status_res, status, last_cmd_status_s);
    return (status);
}

void execute_top_level(t_ast_node *tree, int *idx, int *input_method, int *should_exit, t_status *last_cmd_status_res)
{
    t_executable_node   exe;
    t_status           res;

    exe = (t_executable_node){.infd = 0, .outfd = -1, .node = tree, .modify_parent_context = true};
    *idx = 0;
    if (get_g_sig()->should_unwind)
        gather_heredocs();
    if (!get_g_sig()->should_unwind)
    {
        res = execute_tree_node();
        if (res.c_c)
        {
            if (*input_method == INP_READLINE)
                ft_eprintf("\n");
            else
                *should_exit = true;
        }
        *last_cmd_status_res = res;
    }
}