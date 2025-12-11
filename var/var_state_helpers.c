/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 22:02:37 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:05:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

int changed;

void change_mail(const char *val)
{
	(void)val;
	changed++;
}

t_var make_atty(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "ATTY\0", 0};
}

t_var make_ifs(const char *ifs)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, (char *)ifs, 0};
}

t_var make_mail(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "MAIL\0", change_mail};
}

t_var make_mailpath(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "MAILPATH\0", change_mail};
}

t_var make_path(const char *path)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, (char *)path, 0};
}

