/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_null.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:52:35 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/11 01:16:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_memory.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include "ft_stdio.h"

void out_of_space(void)
{
	//sh_error("Out of space");
	dprintf(STDERR_FILENO, "Out of space");
# if DEBUG
	ft_abort();
#else
	exit(ENOMEM);
#endif
}

void *check_null(void *p)
{
	if (!p)
		out_of_space();
	return (p);
}
