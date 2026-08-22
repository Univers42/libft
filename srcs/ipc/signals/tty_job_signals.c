/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty_job_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 10:12:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/08/22 10:12:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "trap.h"

/* Read a signal's current disposition WITHOUT changing it.

   get_orig_sig() cannot be used here. It fetches by installing SIG_DFL and
   keeping whatever came back -- fine for SIGQUIT and SIGTERM, which the
   shell re-arms straight afterwards, and wrong for the three tty signals:
   a shell that leaves SIGTSTP at SIG_DFL suspends itself the moment a
   child touches the terminal. So: swap, look, swap back. */
static void	save_tty_sig(int sig)
{
	t_sig_handler	handler;

	if (get_g_sig()->original_signals[sig]
		!= (t_sig_handler)IMPOSSIBLE_TRAP_HANDLER)
		return ;
	handler = set_signal_handler(sig, SIG_DFL);
	if (handler == SIG_ERR)
		return ;
	set_signal_handler(sig, handler);
	set_orig_sig(sig, handler);
}

/* Remember how SIGTSTP/SIGTTIN/SIGTTOU looked before this shell touched
   them, so job control can hand them back to a child unchanged.

   Fetched once. A second call would run after the shell installed its own
   job-control handlers and would record THOSE as the originals -- which is
   precisely how a shell forgets how to give the terminal back.

   Non-interactive shells do not ask the kernel: nobody installed these on
   our behalf, so reading them records whatever the parent happened to
   leave behind and then propagates it into every child. SIG_DFL is both
   what bash records and what a child expects. */
void	get_original_tty_job_signals(void)
{
	static int	fetched;

	if (fetched)
		return ;
	fetched = 1;
	if (get_g_sig()->interactive)
	{
		save_tty_sig(SIGTSTP);
		save_tty_sig(SIGTTIN);
		save_tty_sig(SIGTTOU);
	}
	else
	{
		set_orig_sig(SIGTSTP, SIG_DFL);
		set_orig_sig(SIGTTIN, SIG_DFL);
		set_orig_sig(SIGTTOU, SIG_DFL);
	}
}
