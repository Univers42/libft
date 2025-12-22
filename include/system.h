/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:04:51 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/22 02:29:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_H
# define SH_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "ft_wctype.h"

//Those functions has to be maybe placed into the libarena implementation
// it's more related to mmemory management, for strings or data structures
typedef union u_smt
{
	int		i;
	char	*cp;
	double	d;
}	t_smt;

typedef struct s_vcs_info
{
	char	*data;
	int		dirty;
	int		ok;
}	t_vcs_info;

// return the size of union - 1. This value is for allocation granurality
static inline size_t	shell_size()
{
	return (sizeof(t_smt) - 1);
}

/**
 * it appears that grabstackstr() will barf such alignments
 * because stalloc() will return a string allocated in a new stackblock
 */
//static inline size_t	shell_align(size_t nbytes)
//{
//	return (nbytes + SHELL_SIZE & ~SHELL_SIZE)
//}
//
static inline void ensure_locale(void)
{
	static int done = 0;

	if (!done)
	{
		ft_setlocale(LC_CTYPE, "");
		done = 1;
	}
}

#endif
