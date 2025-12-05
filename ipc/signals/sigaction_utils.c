/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigaction_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 21:41:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 21:41:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "trap.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "ft_memory.h"

static inline void posix_zero(sigset_t *s)
{
	if (s)
		ft_memset(s, 0, sizeof(*s));
}

static inline void posix_add(sigset_t *s, int sig)
{
	size_t word;
	size_t bit;

	if (!s || sig <= 0 || sig >= NSIG)
		return;
	word = (size_t)(sig - 1) / (8 * sizeof(unsigned long));
	bit = (size_t)(sig - 1) % (8 * sizeof(unsigned long));
	((unsigned long *)s)[word] |= (1UL << bit);
}

static inline int posix_ismember(const sigset_t *s, int sig)
{
	size_t word;
	size_t bit;

	if (!s || sig <= 0 || sig >= NSIG)
		return (0);
	word = (size_t)(sig - 1) / (8 * sizeof(unsigned long));
	bit = (size_t)(sig - 1) % (8 * sizeof(unsigned long));
	return (((const unsigned long *)s)[word] >> bit) & 1UL;
}

/* Convert t_sigset mask -> POSIX sigset_t */
static void build_posix_set(t_sigset mask, sigset_t *dst)
{
	int sig;

	posix_zero(dst);
	sig = 1;
	while (sig < NSIG)
	{
		if (mask & (1U << (sig - 1)))
			posix_add(dst, sig);
		sig++;
	}
}

/* Convert POSIX sigset_t -> t_sigset mask */
static void to_mask(const sigset_t *pset, t_sigset *out)
{
	int sig;
	t_sigset mask;

	mask = 0;
	sig = 1;
	while (sig < NSIG)
	{
		if (posix_ismember(pset, sig))
			mask |= (1U << (sig - 1));
		sig++;
	}
	*out = mask;
}

t_sig_handler set_signal_handler(int sig, t_sig_handler handler)
{
	struct sigaction act;
	struct sigaction old;
	t_sigset empty_mask;

	if (sig <= 0 || sig >= NSIG)
		return (SIG_ERR);
	ft_sigemptyset(&empty_mask);
	build_posix_set(empty_mask, &act.sa_mask);
	act.sa_handler = handler;
	act.sa_flags = 0;
	if (sigaction(sig, &act, &old) < 0)
		return (SIG_ERR);
	return (old.sa_handler);
}

// /* BSD-style sigblock implemented via sigprocmask */
// static int sigblock_compat(int newmask)
// {
// 	sigset_t set;
// 	sigset_t oset;
// 	t_sigset mask;

// 	mask = (t_sigset)newmask;
// 	build_posix_set(mask, &set);
// 	if (sigprocmask(SIG_BLOCK, &set, &oset) < 0)
// 		return (-1);
// 	to_mask(&oset, &mask);
// 	return ((int)mask);
// }

/* BSD-style sigsetmask implemented via sigprocmask */
// static int sigsetmask_compat(int newmask)
// {
// 	sigset_t set;
// 	sigset_t oset;
// 	t_sigset mask;

// 	mask = (t_sigset)newmask;
// 	build_posix_set(mask, &set);
// 	if (sigprocmask(SIG_SETMASK, &set, &oset) < 0)
// 		return (-1);
// 	to_mask(&oset, &mask);
// 	return ((int)mask);
// }

/* Real implementation: block/unblock signals and return old mask */
int ft_sigprocmask(int operation, t_sigset *newset, t_sigset *oldset)
{
	sigset_t posix_new;
	sigset_t posix_old;
	sigset_t *nptr;

	nptr = NULL;
	if (newset)
	{
		build_posix_set(*newset, &posix_new);
		nptr = &posix_new;
	}
	if (sigprocmask(operation, nptr, &posix_old) < 0)
		return (-1);
	if (oldset)
		to_mask(&posix_old, oldset);
	return (0);
}

/* Real implementation: suspend process until a signal arrives */
int ft_sigsuspend(const t_sigset *set)
{
	sigset_t posix_set;
	t_sigset mask;

	mask = 0;
	if (set)
		mask = *set;
	build_posix_set(mask, &posix_set);
	return (sigsuspend(&posix_set));
}
