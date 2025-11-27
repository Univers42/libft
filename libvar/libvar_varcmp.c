/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libvar_varcmp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:06:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:41:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

int	libvar_varcmp(const char *p, const char *q)
{
	int	c;
	int	d;

	while (1)
	{
		c = *p;
		d = *q;
		if (c != d)
			break ;
		if (!c || c == '=')
			break ;
		p++;
		q++;
	}
	if (c == '=')
		c = 0;
	if (d == '=')
		d = 0;
	return (c - d);
}
