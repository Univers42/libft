/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_null.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:52:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 02:13:05 by dlesieur         ###   ########.fr       */
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
	ft_dprintf(STDERR_FILENO, "Out of space");
	exit(ENOMEM);
}

void *check_null(void *p)
{
	if (!p)
		out_of_space();
	return (p);
}