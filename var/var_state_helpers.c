/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_state_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 16:04:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:16:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"
# include "glob.h"

static void clear_cmd_entry()
{
	t_tblentry **tblp;
	t_tblentry **pp;
	t_tblentry *cmdp;

	intoff();
	tblp = cmdtable;
	while (tblp < &cmdtable[CMDTABLESIZE])
	{
		pp = tblp;
		cmdp = *pp;
		while (cmdp != NULL)
		{
			if (cmdp->cmd_type == CMDNORMAL || (cmdp->cmd_type == CMDBUILTIN && (cmdp->param.cmd->flags & BUILTIN_REGULAR) && builtinloc > 0))
			{
				*pp = cmdp->next;
				xfree(cmdp);
			}
			else
				pp = &cmdp->next;
		}
		tblp++;
	}
	inton();
}

int changed;

void change_mail(const char *val)
{
	(void)val;
	changed++;
}

/**
 * Fix command hash table when Path changed
 * called before PATH is changed. The argument is the new value of pathval()
 * still returns the old value at this point. called with interrupts off.
 */
static void change_path(const char *newval)
{
	const char *new;
	int idx;
	int bltin;

	new = newval;
	idx = 0;
	bltin = -1;
	while (1)
	{
		if (*new == '%' && prefix(new + 1, "builtin"))
		{
			bltin = idx;
			break;
		}
		new = ft_strchr(new, ':');
		if (!new)
			break;
		idx++;
		new++;
	}
	builtinloc = bltin;
	clear_cmd_entry();
}

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
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "MAIL\0", change_mail};
}

t_var make_mailpath(void)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED | VUNSET, "MAILPATH\0", change_mail};
}

t_var make_path(const char *path)
{
	return (t_var){NULL, VSTR_FIXED | VTEXT_FIXED, path, change_path};
}
