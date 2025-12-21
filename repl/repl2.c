/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:35:47 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:35:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdbool.h>
#include "ft_stdio.h"
#include "ipc.h"
#include "var.h"
#include "ipc.h"
#include "lexer.h"
#include <ctype.h> 
#include "ft_wctype.h" 
#include <signal.h> 
#include "trap.h"

void	repl_parent_sigint(int sig)
{
	(void)sig;
}

t_repl_config	**repl_conf_singleton(void)
{
	static t_repl_config	*ptr = NULL;

	return (&ptr);
}

void	set_repl_config(t_repl_config *conf)
{
	*repl_conf_singleton() = conf;
}

void	free_all_state(t_api_readline *shell)
{
	free(shell->input.buff);
	shell->input = (t_dyn_str){0};
	free(shell->last_cmd_status_s);
	free(shell->pid);
	free(shell->context);
	free(shell->base_context);
	shell->context = 0;
	shell->base_context = 0;
	free(shell->rl.str.buff);
	free_hist(&shell->hist);
	free(shell->cwd.buff);
}
