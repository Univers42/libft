/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_err.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:17:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 02:27:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"
#include "trap.h"

/* declare err_exit locally to avoid implicit-declaration during build */
extern void err_exit(const char *name, const char *msg, const void *ctx, int err);

void rl_err(char *name, char *str, int err)
{
	/* call wrapper to avoid conflicting external symbol */
	rl_reset_shell_mode();
	rl_set_mode(&g_rl.start_mode);
	err_exit(name, str, NULL, err);
}
