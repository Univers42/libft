/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 16:04:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 16:37:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

t_var make_atty(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "ATTY\0", 0};
}

t_var make_ifs(const char *ifs)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, ifs, 0};
}

t_var make_mail(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "MAIL\0", changemail};
}

t_var make_mailpath(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "MAILPATH\0", changemail};
}

t_var make_path(const char *path)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, path, changepath};
}

