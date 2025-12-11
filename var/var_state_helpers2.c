/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_helpers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:02:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:05:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

static void set_histsize(char *hs)
{
	(void)hs;
	/* Stub: history management is shell-specific.
	   Real implementation would call history(hist, &he, NULL, histsize). */
}

t_var make_ps4(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, "PS4=+ ", 0};
}

/* Match the public/header prototype: optind is const char *, resetfn is void(*)(void).
   t_var.func expects a callback taking (const char *), so cast resetfn accordingly
   when storing it into the t_var structure. */
t_var make_optind(const char *optind, void (*resetfn)(void))
{
	return (t_var){
		NULL,
		VSTR_FIXED | VTEXT_FIXED,
		(char *)optind, /* t_var.text is char*, cast away const here (stored as static text) */
		(void (*)(const char *))resetfn};
}

t_var make_lineno(const char *lineno)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, (char *)lineno, 0};
}

t_var make_term(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "TERM\0", 0};
}

t_var make_histsize(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "HISTSIZE\0", (void (*)(const char *))set_histsize};
}
