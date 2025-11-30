/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gets.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:29:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/30 21:18:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "trap.h"

void get_all_original_signals(void)
{
	register int i;

	i = 0;
	while (++i < NSIG)
		get_original_signal(i);
}

void get_orig_sig(int sig)
{
	t_sig_handler handler;

	if (sig <= 0 || sig >= NSIG)
		return;
	handler = set_signal_handler(sig, SIG_DFL);
	/* store as function pointer (no void* creeping in) */
	get_g_sig()->original_signals[sig] = handler;
	/* compare as function pointers */
	if (get_g_sig()->original_signals[sig] == SIG_IGN)
		get_g_sig()->sigmodes[sig] |= SIG_HARD_IGNORE;
}

void get_original_signal(int sig)
{
	if (sig > 0 && sig < NSIG && get_g_sig()->original_signals[sig] == IMPOSSIBLE_TRAP_HANDLER)
		get_orig_sig(sig);
}
