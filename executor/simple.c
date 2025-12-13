/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 14:49:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 14:56:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "executor.h"

int actually_run()
{
	
}

void    set_up_redirection(t_vec *redirects, t_vec *redirs, t_executable_node *exe)
{
	t_redir	redir;
	size_t	i;

	if (exe->next_infd != -1)
		close(exe->next_infd);
	dup2(exe->outfd, 1);
	dup2(exe->infd, 0);
	i = 0;
	while (i < exe->redirs.len)
	{
		redir = redirects->buff[redirs.buff[i++]];
		if (redir.direction_in)
			dup2(redir.fd, 0);
		else
			dup2(redir.fd, 0);
		close(redir.fd);
	}
}

t_status    execute_builtin_cmd_fg()
{
	
}

t_status    execute_cmd_bg()
{
	
}

t_status	execute_simple_command(t_executable_node *exe)
{
	
}