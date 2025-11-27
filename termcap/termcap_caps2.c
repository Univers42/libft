/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap_caps2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 21:19:27 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 21:20:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termcap.h"

void	tc_restore_cursor(void)
{
	char	*rc;

	rc = tgetstr("rc", NULL);
	if (rc)
		tputs(rc, 1, (int (*)(int))putchar);
	else
		write(1, "\0338", 2);
}

/* bold/attribute toggles */
void	tc_enter_bold(void)
{
	char	*md;

	md = tgetstr("md", NULL);
	if (md)
		tputs(md, 1, (int (*)(int))putchar);
	else
		write(1, "\033[1m", 4);
}

void	tc_exit_attr(void)
{
	char	*me;

	me = tgetstr("me", NULL);
	if (me)
		tputs(me, 1, (int (*)(int))putchar);
	else
		write(1, "\033[0m", 4);
}
