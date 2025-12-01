/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_helpers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 16:04:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 01:49:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

typedef int t_event;
void *hist;

/* Forward-declare history to avoid implicit declaration warnings.
   Signature matches usage: history(hist, &he, NULL, histsize). */
extern void history(void *hist, t_event *he, void *unused, int histsize);

static void set_histsize(const char *hs)
{
	int histsize;
	t_event he;

	if (hist != NULL)
	{
		histsize = ft_atoi(hs);
		if (hs == NULL || *hs == '\0' || histsize < 0)
			histsize = 100;
		history(hist, &he, /*H_SETSIZE*/ NULL, histsize);
	}
}

t_var make_ps4(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, "PS4=+ ", 0};
}

/* Change: accept a void-returning reset callback (matches private_var.h).
   Store it into t_var.func with an explicit cast to the expected type. */
t_var make_optind(const char *optind, void (*resetfn)(void))
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, optind,
				   (void (*)(const char *))resetfn};
}

t_var make_lineno(const char *lineno)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, lineno, 0};
}

t_var make_term(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "TERM\0", 0};
}
t_var make_histsize(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "HISTSIZE\0", set_histsize};
}
