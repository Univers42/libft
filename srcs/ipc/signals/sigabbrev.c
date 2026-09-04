/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigabbrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 20:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 20:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ft_sigabbrev: symbolic abbreviation of a signal number -- "SEGV", "FPE",
** "ILL" -- the text glibc's sigabbrev_np() returns, without the glibc 2.32
** dependency.
**
** The rest of this module maps names the other way (decode_signal) or
** describes a signal with strsignal(), which may translate, may allocate,
** and is not async-signal-safe. This is a static table and touches nothing
** else, so it is safe to call from inside a signal handler -- which is
** exactly where a crash reporter needs a signal's name.
*/

#include "trap.h"

typedef struct s_sigabbrev
{
	int			num;
	const char	*name;
}	t_sigabbrev;

static const t_sigabbrev	g_sigabbrev[] = {
{SIGHUP, "HUP"}, {SIGINT, "INT"}, {SIGQUIT, "QUIT"}, {SIGILL, "ILL"},
{SIGTRAP, "TRAP"}, {SIGABRT, "ABRT"}, {SIGBUS, "BUS"}, {SIGFPE, "FPE"},
{SIGKILL, "KILL"}, {SIGUSR1, "USR1"}, {SIGSEGV, "SEGV"}, {SIGUSR2, "USR2"},
{SIGPIPE, "PIPE"}, {SIGALRM, "ALRM"}, {SIGTERM, "TERM"}, {SIGCHLD, "CHLD"},
{SIGCONT, "CONT"}, {SIGSTOP, "STOP"}, {SIGTSTP, "TSTP"}, {SIGTTIN, "TTIN"},
{SIGTTOU, "TTOU"}, {SIGURG, "URG"}, {SIGXCPU, "XCPU"}, {SIGXFSZ, "XFSZ"},
{SIGVTALRM, "VTALRM"}, {SIGPROF, "PROF"}, {SIGWINCH, "WINCH"},
{SIGIO, "IO"}, {SIGSYS, "SYS"}
};

const char	*ft_sigabbrev(int sig)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_sigabbrev) / sizeof(g_sigabbrev[0]))
	{
		if (g_sigabbrev[i].num == sig)
			return (g_sigabbrev[i].name);
		i++;
	}
	return (NULL);
}
