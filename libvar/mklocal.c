/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mklocal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/28 19:22:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include <string.h>

static t_localvar	*handle_special_opt(void);
static void			setup_new_local(t_localvar *lvp, char *name, int flags);
static void			setup_existing_local(t_localvar *lvp, t_var *vp,
						char *name, int flags);
static t_localvar	*handle_var_opt(char *name, int flags);

void	mklocal(char *name, int flags)
{
	t_localvar	*lvp;
	t_var		*vp;

	intoff();
	lvp = 
}