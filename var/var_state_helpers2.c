/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_helpers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 16:04:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 18:07:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

static void set_histsize(const char *hs)
{
	(void)hs;
	/* Stub: history management is shell-specific.
	   Real implementation would call history(hist, &he, NULL, histsize). */
}

t_var make_ps4(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, "PS4=+ ", 0};
}

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
