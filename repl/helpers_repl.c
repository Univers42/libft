/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_repl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:35:18 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:35:18 by marvin           ###   ########.fr       */
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

t_repl_config *get_repl_config(void)
{
	return *repl_conf_singleton();
}

t_api_readline **repl_state_singleton(void)
{
	static t_api_readline *ptr = NULL;
	return &ptr;
}

void set_repl_state(t_api_readline *state)
{
	*repl_state_singleton() = state;
}

void get_repl_state(t_api_readline *out)
{
	t_api_readline *state = *repl_state_singleton();
	if (state && out)
		*out = *state;
}


t_api_readline *get_repl_state_ptr(void)
{
	return *repl_state_singleton();
}
