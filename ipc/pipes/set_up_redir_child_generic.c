/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_up_redir_child_generic.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 00:48:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 14:47:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipe.h"

/**
 * used to set up a pipeline child's fds.
 * 
 */
void	setup_redir_child_generic(bool is_last, t_redir_pair *node, int *outfd2, int (*pp)[2])
{
	if (!is_last)
	{
		if (pipe(*pp))
			critical_error_errno_context("pipe");
		node->outfd = (*pp)[1];
		node->next_infd = (*pp)[0];
	}
	else
	{
		node->next_infd = -1;
		node->outfd = dup(*outfd2);
	}
}

void	execute_pipeline_children()
{
	size_t	i;
	
	int		pp[2];

	i = 0;
	curr_exe = (){};
	curr_exe.infd = dup(exe->infd);
	curr_exe.modify_parent_context = exe->node->children.len == 1;
	while (i < exe->node->children.len)
	{
		curr_exe = (t_executable_node)
	}
}
